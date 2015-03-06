#ifndef _GRAPHICS_SERVICE_H_
#define _GRAPHICS_SERVICE_H_

#include <EGL/egl.h>

struct android_app;

class Graphics
{
public:
	Graphics(android_app* aApplication);
	~Graphics();

	int32_t getWidth() const;
	int32_t getHeight() const;

	int32_t start();
	void stop();
	int32_t update();

private:
	android_app* mApplication;

	int32_t mWidth, mHeight;
	EGLDisplay mDisplay;
	EGLSurface mSurface;
	EGLContext mContext;
};

#endif
