#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#ifndef ANDROID
#include "SDL.h"
#else
#define Uint32 uint32_t
#endif
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
#ifndef ANDROID
		/** Runs main loop
		* @returns Result error code
		*/
		int start();
#endif
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
#ifndef ANDROID
		virtual void on_key_down(SDL_Keysym const key);
#else
		virtual int32_t on_key_down(unsigned char const key) = 0;
#endif

	private:
#ifndef ANDROID
		/** SDL window object */
		SDL_Window* m_window;

		/** SDL renderer object */
		SDL_Renderer* m_renderer;

		/** SDL texture we are using as a framebuffer */
		SDL_Texture* m_sdl_target_texture;
#endif
#ifdef ANDROID
	protected:
#endif
		/** Texture we are using as a framebuffer, gets copied into according SDL_Texture to be shown on a screen */
		texture m_target_texture;
#ifdef ANDROID
	private:
#endif
		/** Rendering pipeline */
		pipeline m_pipeline;
#ifdef ANDROID
	protected:
#endif
		/** Delay between frames to stick to the target framerate */
		Uint32 m_target_framerate_delay;
#ifdef ANDROID
	private:
#endif
	};
}

#endif
