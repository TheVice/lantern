
#include <android/api-level.h>
#if __ANDROID_API__ > 8

#include "androidApp.h"
#include "internalApp.h"
#include "gles1x.h"
#include "misc.h"

internalApp* androidApp::mLanternApp = nullptr;

androidApp::androidApp(android_app* aApplication) :
	mApplication(aApplication),
	mEnabled(false),
	mQuit(false),
	mTextureName { 0 },
    mGraphics(aApplication)
{
	mApplication->onAppCmd = activityCallback;
	mApplication->onInputEvent = inputCallback;
	mApplication->userData = this;
}

androidApp::~androidApp()
{
}

void androidApp::start()
{
	int32_t result = 0;
	int32_t events = 0;
	android_poll_source* source = nullptr;
	app_dummy();
	info("Starting event loop");

	while (true)
	{
		while ((result = ALooper_pollAll(mEnabled ? 0 : -1, NULL, &events,
		                                 (void**) &source)) >= 0)
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
			if (onStep() != STATUS_OK)
			{
				info("onStep() return non STATUS_OK\nExiting event loop");
				mQuit = true;
				ANativeActivity_finish(mApplication->activity);
			}
		}
	}
}

int32_t androidApp::onActivate()
{
	info("Activating App");
	ANativeWindow* window = mApplication->window;

	if (ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBX_8888) < 0)
	{
		info("ANativeWindow_setBuffersGeometry - Activating App KO");
		return STATUS_KO;
	}

	if (mGraphics.start() == STATUS_OK)
	{
		info("mGraphicsService started");
		reshape(mGraphics.getWidth(), mGraphics.getHeight());
		initTexture(&mTextureName);
	}
	else
	{
		info("mGraphicsService failed to start");
		return STATUS_KO;
	}

	info("Activating App OK");
	return STATUS_OK;
}

void androidApp::onDeactivate()
{
	info("Deactivating App");

	if (mLanternApp != nullptr)
	{
		delete mLanternApp;
		mLanternApp = nullptr;
	}

	releaseTexture(&mTextureName);
	mGraphics.stop();
	info("App deactivated");
}

int32_t androidApp::onStep()
{
	info("App starting step");
	mLanternApp->start(mTextureName);

	if (mGraphics.update() == STATUS_OK)
	{
		info("App step done OK");
		return STATUS_OK;
	}

	info("App step done KO");
	return STATUS_KO;
}

void androidApp::activate()
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

void androidApp::deactivate()
{
	if (mEnabled)
	{
		onDeactivate();
		mEnabled = false;
	}
}

void androidApp::processingActivityEvent(int32_t aCommand)
{
	switch (aCommand)
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

void androidApp::activityCallback(android_app* aApplication, int32_t aCommand)
{
	androidApp& appClass = *(androidApp*) aApplication->userData;
	appClass.processingActivityEvent(aCommand);
}

int32_t androidApp::inputCallback(android_app* aApplication,
                                  AInputEvent* aEvent)
{
	if (AInputEvent_getType(aEvent) == AINPUT_EVENT_TYPE_KEY)
	{
		int32_t keyVal = AKeyEvent_getKeyCode(aEvent);
		return mLanternApp->on_key_down(keyVal);
	}

	return 0;
}

#endif
