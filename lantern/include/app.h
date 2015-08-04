#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#ifdef WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "pipeline.h"

#ifdef WIN32
BOOL winGlInit(HWND hWnd, HDC& hdc, HGLRC& hglrc);
void winGlRelease(HWND hWnd, HDC& hdc, HGLRC& hglrc);
#else
unsigned long getTimer();
#endif
void reshape(GLsizei width, GLsizei height);
void initTexture(GLuint* texName);
void dispalyTexture(GLuint texName, GLuint imageWidth, GLuint imageHeight, const GLubyte* image);
void releaseTexture(GLuint* texName);

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
		virtual void on_key_down(unsigned char const key) = 0;

	private:
		/** Texture we are using as a framebuffer, gets copied into according OpenGL texture to be shown on a screen */
		texture m_target_texture;

		/** Rendering pipeline */
		pipeline m_pipeline;

		/** Delay between frames to stick to the target framerate */
		int m_target_framerate_delay;

		/** OpenGL texture we are using as a framebuffer */
		GLuint m_texName;

#ifdef WIN32
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hglrc;
		LARGE_INTEGER mFrequency;
		static BOOL winGlInit(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		static void winGlRelease(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		ATOM MyRegisterClass(HINSTANCE hInstance);
		BOOL InitInstance(HINSTANCE aHinstance, int aCmdShow, UINT aWidth, UINT aHeight, HWND& aHwnd);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#else
		Display* m_dpy;
		GLXContext m_glc;
		Window m_win;
#endif
		static void reshape(GLsizei width, GLsizei height);
		static void initTexture(GLuint* texName);
		static void dispalyTexture(GLuint texName, GLuint imageWidth, GLuint imageHeight, const GLubyte* image);
		static void releaseTexture(GLuint* texName);
	};
}

#endif
