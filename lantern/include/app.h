#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#ifdef WIN32

#include <windows.h>

//typedef LRESULT (* message_callback)(HWND, WPARAM, LPARAM);
//struct message_handler
//{
//  UINT message;
//  message_callback handler;
//};
#else
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif


#include "bitmap_painter.h"

namespace lantern
{
	class app
	{
	public:
		app(unsigned int width, unsigned int height);
		virtual ~app();

		int start();

	protected:
		virtual void update() = 0;
//#ifdef WIN32
//		message_handler* m_handlers;
//		int m_handlers_count;
//#endif

	private:
		bitmap_painter m_painter;
#ifdef WIN32
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hglrc;
		ATOM MyRegisterClass(HINSTANCE hInstance);
		BOOL InitInstance(HINSTANCE aHinstance, int aCmdShow, UINT aWidth, UINT aHeight, HWND& aHwnd);
		static BOOL winGlInit(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		static void winGlRelease(HWND hWnd, HDC& hdc, HGLRC& hglrc);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#else
		Display* m_dpy;
		GLXContext m_glc;
		Window m_win;
#endif
		static void init();
	};
}

#endif
