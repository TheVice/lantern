#ifndef ANDROID_APP_H
#define ANDROID_APP_H

#include "app.h"
#include <android_native_app_glue.h>
#include <android/asset_manager_jni.h>

class androidApp;

class internalApp : public lantern::app
{
	friend androidApp;
public:
	/** Initializes application so that it is ready to start running main loop
	* @param aWidth Window and framebuffer texture width
	* @param aHeight Window and framebuffer texture height
	*/
	internalApp(unsigned int const aWidth, unsigned int const aHeight);

	/** Uninitializes application */
	virtual ~internalApp();

	/** Runs main loop
	* @returns Result error code
	* @param aBuffer address to window buffer
	* @param aBufferLength buffer size in bytes
	*/
	int start(void* aBuffer, size_t aBufferLength);
private:
	unsigned long mLastFrameTime;
	unsigned long mTimeAccumulator;
	unsigned int mFramesAccumulator;
};

class androidApp
{
public:
	/** Initializes application so that it is ready to start running main loop
	* @param adress for android_app struct
	*/
	androidApp(android_app* aApplication);

	/** Uninitializes application */
	virtual ~androidApp();

	/** Runs main loop */
	void start();

	/** Permit default constructor */
	androidApp() = delete;

	/** Permit copy constructor */
	androidApp(const androidApp& aRhs) = delete;

	/** Permit assign operator */
	androidApp& operator=(const androidApp& aRhs) = delete;

private:
	android_app* mApplication;
	bool mEnabled;
	bool mQuit;

protected:
	static internalApp* mLanternApp;
	ANativeWindow_Buffer mWindowBuffer;

	virtual int32_t onActivate();

private:
	void onDeactivate();
	int32_t onStep();
	void activate();
	void deactivate();
	void processingActivityEvent(int32_t aCommand);
	static void activityCallback(android_app* aApplication, int32_t aCommand);
	static int32_t inputCallback(android_app* aApplication, AInputEvent* aEvent);

	void onStart() {};
	void onResume() {};
	void onPause() {};
	void onStop() {};
	void onDestroy() {};

	void onSaveState(void** aData, size_t* aSize) {};
	void onConfigurationChanged() {};
	void onLowMemory() {};

	void onCreateWindow() {};
	void onDestroyWindow() {};
	void onGainFocus() {};
	void onLostFocus() {};

};

void info(const char* aMessage, ...);
void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm,
                                       jobject aClazz);
void unpackResourcesFromApk(AAssetManager* aManager);
unsigned long getTimer();

static const int32_t STATUS_OK = 0;
static const int32_t STATUS_KO = -1;

#endif
