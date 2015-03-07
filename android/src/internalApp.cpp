
#include "internalApp.h"
#include "gles1x.h"
#include <unistd.h>

internalApp::internalApp(unsigned int const aWidth,
                         unsigned int const aHeight) :
	lantern::app(aWidth, aHeight),
	mLastFrameTime{ 0 },
	mTimeAccumulator{ 0 },
	mFramesAccumulator{ 0 }
{
	// Time when last frame was executed
	mLastFrameTime = getTimer();
}

internalApp::~internalApp()
{
}

int internalApp::start(GLuint aTextureName)
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
	dispalyTexture(aTextureName, m_target_texture.get_width(),
	               m_target_texture.get_height(), m_target_texture.get_data());
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

unsigned long getTimer()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_nsec / 1000000 + now.tv_sec * 1000;
}
