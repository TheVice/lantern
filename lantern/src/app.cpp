#include <iostream>
#include <stdexcept>
#include "app.h"

using namespace lantern;

#ifdef ANDROID
#include <android/log.h>
#include <stdarg.h>

void info(const char* message, ...)
{
	va_list varArgs;
	va_start(varArgs, message);
	__android_log_vprint(ANDROID_LOG_INFO, "rotating_car_app", message, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "rotating_car_app", "\n");
	va_end(varArgs);
}

app::app(android_app* application) : m_painter{nullptr},
									 mApplication{application},
									 mEnabled{false},
									 mQuit{false},
									 mTimerStart{0},
									 mTime_accumulator_millis{0},
									 mFps{0}
{
	mApplication->onAppCmd = activityCallback;
	mApplication->userData = this;
}

app::~app()
{
}

void app::start()
{
	app_dummy();
	info("Starting event loop");

	while (true)
	{
		int32_t events;
		android_poll_source* source;

		while (ALooper_pollAll(mEnabled ? 0 : -1, NULL, &events, (void**)&source) >= 0)
		{
			if (source != NULL)
			{
				info("Processing an event");
				source->process(mApplication, source);
			}

			if (mApplication->destroyRequested)
			{
				info("Exiting event loop");
				return;
			}
		}
		if ((mEnabled) && (!mQuit))
		{
			if(onStep() != STATUS_OK)
			{
				info("onStep() return non STATUS_OK\nExiting event loop");
				mQuit = true;
				ANativeActivity_finish(mApplication->activity);
			}
		}
	}
}

bitmap_painter& app::get_painter()
{
	return *(bitmap_painter*)m_painter;
}

int32_t app::onActivate()
{
	info("Activating App");
	ANativeWindow* window = mApplication->window;
	if (ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBX_8888) < 0)
	{
		info("ANativeWindow_setBuffersGeometry - Activating App KO");
		return STATUS_KO;
	}

	//Needs to lock the window buffer to get its properties.
	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0)
	{
		ANativeWindow_unlockAndPost(window);
	}
	else
	{
		info("ANativeWindow_lock - Activating App KO");
		return STATUS_KO;
	}

	m_painter = new bitmap_painter{static_cast<unsigned int>(mWindowBuffer.width),
								   static_cast<unsigned int>(mWindowBuffer.height)};

	mTimerStart = getTimer();
	info("Activating App OK");
	return STATUS_OK;
}

void app::onDeactivate()
{
	info("Deactivating App");
	delete m_painter;
	m_painter = nullptr;
	info("App deactivated");
}

int32_t app::onStep()
{
	info("App starting step");


	ANativeWindow* window = mApplication->window;
	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0)
	{
		// Clear texture
		m_painter->clear(0);

		// Calculate time since last frame
		//
		unsigned long currentTime = getTimer();
		unsigned long delta_since_last_frame_millis = currentTime - mTimerStart;

		// Execute frame
		frame(delta_since_last_frame_millis / 1000.0);

		// Sum up passed time
		mTime_accumulator_millis += delta_since_last_frame_millis;

		// Present texture
		//TODO: filled mWindowBuffer.bits

		ANativeWindow_unlockAndPost(window);

		++mFps;

		if (mTime_accumulator_millis >= 1000)
		{
//#ifdef LANTERN_DEBUG_OUTPUT_FPS
			info("FPS: %i", mFps);
//#endif
			mTime_accumulator_millis = 0;
			mFps = 0;
		}

		info("App step done OK");
		// Save last frame time
		mTimerStart = getTimer();
		return STATUS_OK;
	}

	info("App step done KO");
	return STATUS_KO;
}

void app::activate()
{
	if ((!mEnabled) && (mApplication->window != NULL))
	{
		mQuit = false;
		mEnabled = true;
		if (onActivate() != STATUS_OK)
		{
			mQuit = true;
			ANativeActivity_finish(mApplication->activity);
		}
	}
}

void app::deactivate()
{
	if (mEnabled)
	{
		onDeactivate();
		mEnabled = false;
	}
}

void app::processingActivityEvent(int32_t command)
{
	switch(command)
	{
		case APP_CMD_CONFIG_CHANGED:
			onConfigurationChanged();
			break;
		case APP_CMD_INIT_WINDOW:
			onCreateWindow();
			break;
		case APP_CMD_DESTROY:
			onDestroy();
			break;
		case APP_CMD_GAINED_FOCUS:
			activate();
			onGainFocus();
			break;
		case APP_CMD_LOST_FOCUS:
			onLostFocus();
			deactivate();
			break;
		case APP_CMD_LOW_MEMORY:
			onLowMemory();
			break;
		case APP_CMD_PAUSE:
			onPause();
			deactivate();
			break;
		case APP_CMD_RESUME:
			onResume();
			break;
		case APP_CMD_SAVE_STATE:
			onSaveState(&mApplication->savedState, &mApplication->savedStateSize);
			break;
		case APP_CMD_START:
			onStart();
			break;
		case APP_CMD_STOP:
			onStop();
			break;
		case APP_CMD_TERM_WINDOW:
			onDestroyWindow();
			deactivate();
			break;
		default:
			break;
	}
}

void app::activityCallback(android_app* application, int32_t command)
{
	app& appClass = *(app*)application->userData;
	appClass.processingActivityEvent(command);
}

unsigned long app::getTimer()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_nsec / 1000000 + now.tv_sec * 1000;
}

const int32_t app::STATUS_OK = 0;
const int32_t app::STATUS_KO = -1;

#else

app::app(unsigned int const width, unsigned int const height)
	: m_window{nullptr},
	  m_renderer{nullptr},
	  m_target_texture{nullptr},
	  m_painter{width, height}
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_window = SDL_CreateWindow(
		"lantern",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_target_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (m_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

app::~app()
{
	if (m_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_target_texture);
		m_target_texture = nullptr;
	}

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}

int app::start()
{
	bool running{true};
	SDL_Event event;

	// Time when last frame was executed
	Uint32 last_frame_time = 0;

	Uint32 time_accumulator = 0;
	unsigned int fps = 0;

	while(running)
	{
		// Process events
		//
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				on_key_down(event.key.keysym);
			}
		}

		// Clear texture
		m_painter.clear(0);

		// Calculate time since last frame
		//
		Uint32 current_time = SDL_GetTicks();
		Uint32 delta_since_last_frame = current_time - last_frame_time;

		// Save last frame time
		last_frame_time = SDL_GetTicks();

		// Execute frame
		frame(delta_since_last_frame / 1000.0f);

		// Sum up passed time
		time_accumulator += delta_since_last_frame;

		// Present texture
		//
		SDL_UpdateTexture(m_target_texture, nullptr, m_painter.get_data(), m_painter.get_pitch());
		SDL_RenderCopy(m_renderer, m_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

		++fps;

		if (time_accumulator >= 1000)
		{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
			std::cout << "FPS: " << fps << std::endl;
#endif
			time_accumulator = 0;
			fps = 0;
		}
	}

	return 0;
}

bitmap_painter& app::get_painter()
{
	return m_painter;
}

void app::on_key_down(SDL_Keysym key)
{

}

#endif
