
#include "androidApp.h"

Graphics::Graphics(android_app* pApplication)
	: mApplication(pApplication),
	  mWidth(0), mHeight(0),
	  mDisplay(EGL_NO_DISPLAY),
	  mSurface(EGL_NO_SURFACE),
	  mContext(EGL_NO_CONTEXT)
{
}

Graphics::~Graphics()
{
}

int32_t Graphics::getWidth() const
{
	return mWidth;
}

int32_t Graphics::getHeight() const
{
	return mHeight;
}

int32_t Graphics::start()
{
	EGLint format = 0, numConfigs = 0;
	EGLConfig config = nullptr;
	const EGLint attributes[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
	};
	mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (mDisplay == EGL_NO_DISPLAY)
	{
		stop();
		return STATUS_KO;
	}

	if (!eglInitialize(mDisplay, NULL, NULL))
	{
		stop();
		return STATUS_KO;
	}

	if (!eglChooseConfig(mDisplay, attributes, &config, 1,
	                     &numConfigs) || (numConfigs <= 0))
	{
		stop();
		return STATUS_KO;
	}

	if (!eglGetConfigAttrib(mDisplay, config,
	                        EGL_NATIVE_VISUAL_ID, &format))
	{
		stop();
		return STATUS_KO;
	}

	ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0,
	                                 format);
	mSurface = eglCreateWindowSurface(mDisplay, config,
	                                  mApplication->window, NULL);

	if (mSurface == EGL_NO_SURFACE)
	{
		stop();
		return STATUS_KO;
	}

	mContext = eglCreateContext(mDisplay, config,
	                            EGL_NO_CONTEXT, NULL);

	if (mContext == EGL_NO_CONTEXT)
	{
		stop();
		return STATUS_KO;
	}

	if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
	    || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
	    || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
	    || (mWidth <= 0) || (mHeight <= 0))
	{
		stop();
		return STATUS_KO;
	}

	return STATUS_OK;
}

void Graphics::stop()
{
	if (mDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
		               EGL_NO_CONTEXT);

		if (mContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(mDisplay, mContext);
			mContext = EGL_NO_CONTEXT;
		}

		if (mSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(mDisplay, mSurface);
			mSurface = EGL_NO_SURFACE;
		}

		eglTerminate(mDisplay);
		mDisplay = EGL_NO_DISPLAY;
	}
}

int32_t Graphics::update()
{
	//onDrawFrame();
	if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE)
	{
		info("Error %d swapping buffers.", eglGetError());
		return STATUS_KO;
	}

	return STATUS_OK;
}
