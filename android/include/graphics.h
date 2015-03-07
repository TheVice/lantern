#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <EGL/egl.h>

struct android_app;

class graphics
{
public:
	graphics(android_app* aApplication);
	~graphics();

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
