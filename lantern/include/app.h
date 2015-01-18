#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#ifdef ANDROID

#include "pipeline.h"
#include <android_native_app_glue.h>
#include <android/asset_manager_jni.h>

void info(const char* message, ...);
void changeDirectoryToAppCacheLocation(JNIEnv* env, JavaVM* vm, jobject clazz);
void unpackResourcesFromApk(AAssetManager* mgr);
unsigned long getTimer();

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
		texture& get_target_texture();
		pipeline& get_pipeline();
		void set_target_framerate(unsigned int const fps);

		virtual void frame(float const delta_since_last_frame) = 0;
		virtual int32_t on_key_down(unsigned char const key) = 0;

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
		texture* m_target_texture;
		pipeline m_pipeline;
		android_app* mApplication;
		bool mEnabled;
		bool mQuit;
		unsigned long m_last_frame_time;
		unsigned long m_time_accumulator;
		unsigned int m_frames_accumulator;
		uint32_t m_target_framerate_delay;

	private:
		void activate();
		void deactivate();
		void processingActivityEvent(int32_t command);
		static void activityCallback(android_app* application, int32_t command);
		static int32_t inputCallback(android_app* application, AInputEvent* event);

	protected:
		static const int32_t STATUS_OK;
		static const int32_t STATUS_KO;
	};
}

#else

#include "SDL.h"
#include "pipeline.h"

namespace lantern
{
	/** Base class for all lantern applications.
	* It handles most part of low level stuff, like initializating SDL library and according objects, running the main loop, etc
	*/
	class app
	{
	public:
		/** Initializes application so that it is ready to start running main loop
		* @param width Window and framebuffer texture width
		* @param height Window and framebuffer texture height
		*/
		app(unsigned int const width, unsigned int const height);

		/** Uninitializes application */
		virtual ~app();

		/** Runs main loop
		* @returns Result error code
		*/
		int start();

	protected:
		/** Gets texture used as a framebuffer
		* @returns Target texture
		*/
		texture& get_target_texture();

		/** Gets rendering pipeline
		* @returns Pipeline
		*/
		pipeline& get_pipeline();

		/** Sets target framerate
		* @param fps Target framerate
		*/
		void set_target_framerate(unsigned int const fps);

		/** Handles every frame changes, gets called from the main loop
		* @param delta_since_last_frame How many seconds passed since last frame
		*/
		virtual void frame(float const delta_since_last_frame) = 0;

		/** Handles pressed key
		* @param key Key that was pressed
		*/
		virtual void on_key_down(SDL_Keysym const key);

	private:
		/** SDL window object */
		SDL_Window* m_window;

		/** SDL renderer object */
		SDL_Renderer* m_renderer;

		/** SDL texture we are using as a framebuffer */
		SDL_Texture* m_sdl_target_texture;

		/** Texture we are using as a framebuffer, gets copied into according SDL_Texture to be shown on a screen */
		texture m_target_texture;

		/** Rendering pipeline */
		pipeline m_pipeline;

		/** Delay between frames to stick to the target framerate */
		Uint32 m_target_framerate_delay;
	};
}

#endif
#endif
