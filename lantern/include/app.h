#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#ifdef ANDROID

#include "bitmap_painter.h"
#include <android_native_app_glue.h>

void info(const char* message, ...);

namespace lantern
{
	class app
	{
	public:
		app(android_app* application);
		virtual ~app();

		// Runs main loop
		void start();

	protected:
		bitmap_painter& get_painter();

		virtual void frame(float delta_since_last_frame) = 0;
		virtual void on_key_down(unsigned char key) = 0;

		virtual int32_t onActivate();
		virtual void onDeactivate();
		virtual int32_t onStep();

		virtual void onStart() {};
		virtual void onResume() {};
		virtual void onPause() {};
		virtual void onStop() {};
		virtual void onDestroy() {};

		virtual void onSaveState(void** data, size_t* size) {};
		virtual void onConfigurationChanged() {};
		virtual void onLowMemory() {};

		virtual void onCreateWindow() {};
		virtual void onDestroyWindow() {};
		virtual void onGainFocus() {};
		virtual void onLostFocus() {};

	protected:
		ANativeWindow_Buffer mWindowBuffer;

	private:
		bitmap_painter* m_painter;
		android_app* mApplication;
		bool mEnabled;
		bool mQuit;
		unsigned long mTimerStart;
		unsigned long mTime_accumulator_millis;
		unsigned int mFps;

	private:
		void activate();
		void deactivate();
		void processingActivityEvent(int32_t command);
		static void activityCallback(android_app* application, int32_t command);
		static unsigned long getTimer();

	protected:
		static const int32_t STATUS_OK;
		static const int32_t STATUS_KO;
	};
}

#else

#include <SDL2/SDL.h>
#include "bitmap_painter.h"

namespace lantern
{
	class app
	{
	public:
		app(unsigned int const width, unsigned int const height);
		virtual ~app();

		// Runs main loop
		int start();

	protected:
		bitmap_painter& get_painter();

		virtual void frame(float delta_since_last_frame) = 0;
		virtual void on_key_down(SDL_Keysym key);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_target_texture;
		bitmap_painter m_painter;
	};
}

#endif
#endif
