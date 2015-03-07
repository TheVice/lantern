#ifndef INTERNAL_APP_H
#define INTERNAL_APP_H

#include "app.h"
#include <GLES/gl.h>

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
	* @param aTextureName texture on which draw
	*/
	int start(GLuint aTextureName);
private:
	unsigned long mLastFrameTime;
	unsigned long mTimeAccumulator;
	unsigned int mFramesAccumulator;
};

unsigned long getTimer();

#endif
