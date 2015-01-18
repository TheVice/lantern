#include <iostream>
#include <stdexcept>
#include "app.h"

using namespace lantern;

#ifdef ANDROID

#include <android/log.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

void info(const char* message, ...)
{
	va_list varArgs;
	va_start(varArgs, message);
	__android_log_vprint(ANDROID_LOG_INFO, "lantern", message, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "lantern", "\n");
	va_end(varArgs);
}

void changeDirectoryToAppCacheLocation(JNIEnv* env, JavaVM* vm, jobject clazz)
{
	vm->AttachCurrentThread(&env, NULL);
	jclass activityClass = env->GetObjectClass(clazz);
	// Get path to cache dir (/data/data/appPackageName/cache)
	jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir",
	                        "()Ljava/io/File;");
	jobject file = env->CallObjectMethod(clazz, getCacheDir);
	jclass fileClass = env->FindClass("java/io/File");
	jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath",
	                            "()Ljava/lang/String;");
	jstring jpath = (jstring)env->CallObjectMethod(file, getAbsolutePath);
	const char* app_dir = env->GetStringUTFChars(jpath, NULL);
	// chdir in the application cache directory
	info("app cache dir: %s", app_dir);
	chdir(app_dir);
	env->ReleaseStringUTFChars(jpath, app_dir);
	vm->DetachCurrentThread();
}

void unpackResourcesFromApk(AAssetManager* mgr)
{
	const char* dir_name = "resources";
	mkdir(dir_name, S_IRWXU | S_IRWXG);
	AAssetDir* assetDir = AAssetManager_openDir(mgr, dir_name);
	const char* filename = (const char*)NULL;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
	{
		char input_path[BUFSIZ];
		strcpy(input_path, dir_name);
		strcat(input_path, "/");
		strcat(input_path, filename);
		AAsset* asset = AAssetManager_open(mgr, input_path, AASSET_MODE_STREAMING);
		char buf[BUFSIZ];
		int nb_read = 0;
		char output_path[BUFSIZ];
		strcpy(output_path, dir_name);
		strcat(output_path, "/");
		strcat(output_path, filename);
		FILE* out = fopen(output_path, "w");

		while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
		{
			fwrite(buf, nb_read, 1, out);
		}

		fclose(out);
		AAsset_close(asset);
	}

	AAssetDir_close(assetDir);
}

unsigned long getTimer()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_nsec / 1000000 + now.tv_sec * 1000;
}

app::app(android_app* application) : m_target_texture{nullptr},
									 mApplication{application},
									 mEnabled{false},
									 mQuit{false},
									 m_last_frame_time{0},
									 m_time_accumulator{0},
									 m_frames_accumulator{0},
									 m_target_framerate_delay(0)
{
	mApplication->onAppCmd = activityCallback;
	mApplication->onInputEvent = inputCallback;
	mApplication->userData = this;
	set_target_framerate(60);
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
		int32_t event;
		android_poll_source* source;

		while (ALooper_pollAll(mEnabled ? 0 : -1, NULL, &event, (void**)&source) >= 0)
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

texture& app::get_target_texture()
{
	return *(texture*)m_target_texture;
}

pipeline& app::get_pipeline()
{
	return m_pipeline;
}

void app::set_target_framerate(unsigned int const fps)
{
	if (fps == 0)
	{
		m_target_framerate_delay = 0;
	}
	else
	{
		m_target_framerate_delay = 1000 / fps;
	}
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

	m_target_texture = new texture{static_cast<unsigned int>(mWindowBuffer.width),
								   static_cast<unsigned int>(mWindowBuffer.height)};

	// Time when last frame was executed
	m_last_frame_time = getTimer();
	info("Activating App OK");
	return STATUS_OK;
}

void app::onDeactivate()
{
	info("Deactivating App");
	if (m_target_texture != nullptr)
	{
		delete m_target_texture;
		m_target_texture = nullptr;
	}
	info("App deactivated");
}

int32_t app::onStep()
{
	info("App starting step");

	ANativeWindow* window = mApplication->window;
	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0)
	{
		// Calculate time since last frame
		//
		unsigned long currentTime = getTimer();
		unsigned long delta_since_last_frame_millis = currentTime - m_last_frame_time;

		// Save last frame time
		m_last_frame_time = getTimer();

		// Clear texture with black
		m_target_texture->clear(0);

		// Execute frame
		frame(delta_since_last_frame_millis / 1000.0);

		// Sum up passed time
		m_time_accumulator += delta_since_last_frame_millis;

		// Present texture on a screen
		//
		memcpy(mWindowBuffer.bits, m_target_texture->get_data(), 4 * mWindowBuffer.width * mWindowBuffer.height);

		ANativeWindow_unlockAndPost(window);

		// Sum up passed frames
		++m_frames_accumulator;

		if (m_target_framerate_delay > 0)
		{
			// Calculate delay we must have to stick to the target framerate
			//
			uint32_t time_required_for_frame = getTimer() - m_last_frame_time;
			int time_to_wait = m_target_framerate_delay - time_required_for_frame;

			if (time_to_wait > 0)
			{
				usleep(static_cast<unsigned long>(time_to_wait / 1000.0));
			}
		}

		// Drop frames and seconds counters to zero every second
		//
		if (m_time_accumulator >= 1000)
		{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
			info("FPS: %i", m_frames_accumulator);
#endif
			m_time_accumulator = 0;
			m_frames_accumulator = 0;
		}

		info("App step done OK");
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

int32_t app::inputCallback(android_app* application, AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
		int32_t key_val = AKeyEvent_getKeyCode(event);
		app& appClass = *(app*)application->userData;
		return appClass.on_key_down(key_val);
	}
	return 0;
}

const int32_t app::STATUS_OK = 0;
const int32_t app::STATUS_KO = -1;

#else

app::app(unsigned int const width, unsigned int const height)
	: m_window{nullptr},
	  m_renderer{nullptr},
	  m_sdl_target_texture{nullptr},
	  m_target_texture{width, height},
	  m_target_framerate_delay(0)
{
	// Initialize SDL library and according objects
	//

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

	m_sdl_target_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (m_sdl_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	set_target_framerate(60);
}

app::~app()
{
	// Clean up SDL library
	//

	if (m_sdl_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_sdl_target_texture);
		m_sdl_target_texture = nullptr;
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
	Uint32 last_frame_time{0};

	Uint32 time_accumulator{0};
	unsigned int frames_accumulator{0};

	while(running)
	{
		// Calculate time since last frame
		//
		Uint32 current_time{SDL_GetTicks()};
		Uint32 delta_since_last_frame{current_time - last_frame_time};

		// Save last frame time
		last_frame_time = SDL_GetTicks();

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

		// Clear texture with black
		m_target_texture.clear(0);

		// Execute frame
		frame(delta_since_last_frame / 1000.0f);

		// Sum up passed time
		time_accumulator += delta_since_last_frame;

		// Present texture on a screen
		//
		SDL_UpdateTexture(m_sdl_target_texture, nullptr, m_target_texture.get_data(), m_target_texture.get_pitch());
		SDL_RenderCopy(m_renderer, m_sdl_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

		// Sum up passed frames
		++frames_accumulator;

		if (m_target_framerate_delay > 0)
		{
			// Calculate delay we must have to stick to the target framerate
			//
			Uint32 time_required_for_frame = SDL_GetTicks() - last_frame_time;
			int time_to_wait = m_target_framerate_delay - time_required_for_frame;

			if (time_to_wait > 0)
			{
				SDL_Delay(static_cast<Uint32>(time_to_wait));
			}
		}

		// Drop frames and seconds counters to zero every second
		//
		if (time_accumulator >= 1000)
		{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
			std::cout << "FPS: " << frames_accumulator << std::endl;
#endif
			time_accumulator = 0;
			frames_accumulator = 0;
		}
	}

	return 0;
}

texture& app::get_target_texture()
{
	return m_target_texture;
}

pipeline& app::get_pipeline()
{
	return m_pipeline;
}

void app::on_key_down(SDL_Keysym const)
{
	// Does not handle any key by default
}

void app::set_target_framerate(unsigned int const fps)
{
	if (fps == 0)
	{
		m_target_framerate_delay = 0;
	}
	else
	{
		m_target_framerate_delay = 1000 / fps;
	}
}

#endif
