#ifndef ANDROID_APP_H
#define ANDROID_APP_H

#include <android/api-level.h>
#if __ANDROID_API__ > 8

#include "graphics.h"
#include <GLES/gl.h>
#include <android_native_app_glue.h>

class internalApp;

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
	GLuint mTextureName;

protected:
	graphics mGraphics;
	static internalApp* mLanternApp;

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

#endif
#endif
