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

#include "bitmap_painter.h"

namespace lantern
{
	class app
	{
	public:
		app(unsigned int const width, unsigned int const height);
		virtual ~app();

		/** Runs main loop
		*/
		int start();

	protected:
		bitmap_painter& get_painter();
		virtual void frame(float delta_since_last_frame) = 0;
		virtual void on_key_down(unsigned char key) = 0;

	private:
		bitmap_painter m_painter;
		GLuint m_texName;
#ifdef WIN32
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hglrc;
		LARGE_INTEGER mFrequency;
		ATOM MyRegisterClass(HINSTANCE hInstance);
		BOOL InitInstance(HINSTANCE aHinstance, int aCmdShow, UINT aWidth, UINT aHeight, HWND& aHwnd);
		static BOOL winGlInit(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		static void winGlRelease(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#else
		Display* m_dpy;
		GLXContext m_glc;
		Window m_win;
		static unsigned long getTimer();
#endif
		static void reshape(int w, int h);
		static void initTexture(GLuint* texName, GLuint imageWidth, GLuint imageHeight, const GLubyte* image);
		static void dispalyTexture(GLuint texName);
	};
}

#endif
