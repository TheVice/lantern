
#include "androidApp.h"
#include <iostream>
#include <stdexcept>
#include <android/log.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

internalApp::internalApp(unsigned int const aWidth, unsigned int const aHeight)
	: lantern::app(aWidth, aHeight),
	  mLastFrameTime {0},
                 mTimeAccumulator {0},
mFramesAccumulator {0}
{
	// Time when last frame was executed
	mLastFrameTime = getTimer();
}

internalApp::~internalApp()
{
}

int internalApp::start(void* aBuffer, size_t aBufferLength)
{
	// Calculate time since last frame
	//
	unsigned long currentTime = getTimer();
	unsigned long deltaSinceLastFrameMillis = currentTime - mLastFrameTime;
	// Save last frame time
	mLastFrameTime = getTimer();
	// Clear texture with black
	m_target_texture.clear(0);
	// Execute frame
	frame(deltaSinceLastFrameMillis / 1000.0);
	// Sum up passed time
	mTimeAccumulator += deltaSinceLastFrameMillis;
	// Present texture on a screen
	//
	memcpy(aBuffer, m_target_texture.get_data(), aBufferLength);
	// Sum up passed frames
	++mFramesAccumulator;

	if (m_target_framerate_delay > 0)
	{
		// Calculate delay we must have to stick to the target framerate
		//
		uint32_t timeRequiredForFrame = getTimer() - mLastFrameTime;
		int timeToWait = m_target_framerate_delay - timeRequiredForFrame;

		if (timeToWait > 0)
		{
			usleep(static_cast<unsigned long>(timeToWait / 1000.0));
		}
	}

	// Drop frames and seconds counters to zero every second
	//
	if (mTimeAccumulator >= 1000)
	{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
		info("FPS: %i", m_frames_accumulator);
#endif
		mTimeAccumulator = 0;
		mFramesAccumulator = 0;
	}

	return 0;
}

internalApp* androidApp::mLanternApp = nullptr;

androidApp::androidApp(android_app* aApplication)
	: mApplication(aApplication), mEnabled(false), mQuit(false)
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
		                                 (void**)&source)) >= 0)
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

	info("App deactivated");
}

int32_t androidApp::onStep()
{
	info("App starting step");
	ANativeWindow* window = mApplication->window;

	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0)
	{
		mLanternApp->start(mWindowBuffer.bits,
		                   4 * mWindowBuffer.width * mWindowBuffer.height);
		ANativeWindow_unlockAndPost(window);
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
	androidApp& appClass = *(androidApp*)aApplication->userData;
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

void info(const char* aMessage, ...)
{
	va_list varArgs;
	va_start(varArgs, aMessage);
	__android_log_vprint(ANDROID_LOG_INFO, "lantern", aMessage, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "lantern", "\n");
	va_end(varArgs);
}

void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm,
                                       jobject aClazz)
{
	aVm->AttachCurrentThread(&aEnv, NULL);
	jclass activityClass = aEnv->GetObjectClass(aClazz);
	// Get path to cache dir (/data/data/appPackageName/cache)
	jmethodID getCacheDir = aEnv->GetMethodID(activityClass, "getCacheDir",
	                        "()Ljava/io/File;");
	jobject file = aEnv->CallObjectMethod(aClazz, getCacheDir);
	jclass fileClass = aEnv->FindClass("java/io/File");
	jmethodID getAbsolutePath = aEnv->GetMethodID(fileClass, "getAbsolutePath",
	                            "()Ljava/lang/String;");
	jstring jpath = (jstring)aEnv->CallObjectMethod(file, getAbsolutePath);
	const char* app_dir = aEnv->GetStringUTFChars(jpath, NULL);
	// chdir in the application cache directory
	info("app cache dir: %s", app_dir);
	chdir(app_dir);
	aEnv->ReleaseStringUTFChars(jpath, app_dir);
	aVm->DetachCurrentThread();
}

void unpackResourcesFromApk(AAssetManager* aManager)
{
	const char* dir_name = "resources";
	mkdir(dir_name, S_IRWXU | S_IRWXG);
	AAssetDir* assetDir = AAssetManager_openDir(aManager, dir_name);
	const char* filename = (const char*)NULL;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
	{
		char input_path[BUFSIZ];
		strcpy(input_path, dir_name);
		strcat(input_path, "/");
		strcat(input_path, filename);
		AAsset* asset = AAssetManager_open(aManager, input_path, AASSET_MODE_STREAMING);
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
