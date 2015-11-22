#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "SDL.h"
#include "renderer.h"

namespace lantern
{
	/** Base class for all lantern applications.
	* It handles most part of low level stuff, like initializating libraries and according objects, running the main loop, etc.
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

		/** Gets FreeType library main object
		* @returns Pointer to FreeType main object
		*/
		FT_Library get_freetype_library() const;

		/** Gets FPS
		* @returns Last saved framerate
		*/
		unsigned int get_last_fps() const;

		/** Gets platform-dependent separator symbol 
		* @returns Separator
		*/
		char get_path_separator() const;

		/** Gets absolute path to resources folder, already contains ending separator
		* @returns Path to resources
		*/
		std::string get_resources_path() const;
		
		/** Gets application instance
		* @returns Instance
		*/
		static app const* get_instance();

	protected:
		/** Gets texture used as a framebuffer
		* @returns Target texture
		*/
		texture& get_target_texture();

		/** Gets rendering pipeline
		* @returns Pipeline
		*/
		renderer& get_renderer();

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
		/** FreeType library main object */
		FT_Library m_freetype_library;

		/** SDL window object */
		SDL_Window* m_window;

		/** SDL renderer object */
		SDL_Renderer* m_sdl_renderer;

		/** SDL texture we are using as a framebuffer */
		SDL_Texture* m_sdl_target_texture;

		/** Texture we are using as a framebuffer, gets copied into according SDL_Texture to be shown on a screen */
		texture m_target_texture;

		/** Rendering pipeline */
		renderer m_renderer;

		/** Delay between frames to stick to the target framerate */
		Uint32 m_target_framerate_delay;

		/** Last saved framerate */
		unsigned int m_last_fps;

		/** Platform-dependent path separator */
		char const m_path_separator;

		/** Absolute path to resources folder */
		std::string m_resources_path;

		/** Instance pointer */
		static app* _instance;
	};
}

#endif
