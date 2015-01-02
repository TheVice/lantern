#include <iostream>
#include <stdexcept>

#include <GL/gl.h>
#include <GL/glu.h>

#include "app.h"

using namespace lantern;

#ifdef WIN32
app::app(unsigned int width, unsigned int height)
	: //m_handlers(nullptr),
      //m_handlers_count(0),
      m_painter{width, height},
      m_hwnd(HWND_DESKTOP),
      m_hdc(0),
      m_hglrc(0)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!MyRegisterClass(hInstance))
	{
		throw std::runtime_error("app::app::MyRegisterClass");
	}

	if (!InitInstance(hInstance, SW_SHOW, width, height, m_hwnd))
	{
		throw std::runtime_error("app::app::InitInstance");
	}

	if (!winGlInit(m_hwnd, m_hdc, m_hglrc))
	{
		throw std::runtime_error("app::app::winGlInit");
	}
	init();
}

app::~app()
{
	winGlRelease(m_hwnd, m_hdc, m_hglrc);
}

int app::start()
{
	MSG msg;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			update();
			SwapBuffers(m_hdc);
		}
	} while (WM_QUIT != msg.message);

	return 0;
}

ATOM app::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	/*wcex.lpfnWndProc	= [] (HWND window, UINT message,
		      WPARAM wparam, LPARAM lparam) -> LRESULT
		{
		  for (auto h = m_handlers; h != m_handlers + m_handlers_count; ++h)
		  {
		    if (message == h->message)
		    {
		      return h->handler(window, wparam, lparam);
		    }
		  }
		  return DefWindowProc(window, message, wparam, lparam);
		};*/
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("szWindowClass");
	wcex.hIconSm		= NULL;
	return RegisterClassEx(&wcex);
}

BOOL app::InitInstance(HINSTANCE aHinstance, int aCmdShow, UINT aWidth, UINT aHeight,
                  HWND& aHwnd)
{
	const DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	const DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	RECT windowRect;
	windowRect.left = 0L;
	windowRect.right = aWidth;
	windowRect.top = 0L;
	windowRect.bottom = aHeight;

	if (!AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle))
	{
		return FALSE;
	}

	aHwnd = CreateWindowEx(dwExStyle, TEXT("szWindowClass"), TEXT("Title"), dwStyle,
	                       CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
	                       windowRect.bottom - windowRect.top,
	                       HWND_DESKTOP, NULL, aHinstance, NULL);

	if (!aHwnd)
	{
		return FALSE;
	}

	ShowWindow(aHwnd, aCmdShow);
	UpdateWindow(aHwnd);
	return TRUE;
}

BOOL app::winGlInit(HWND hWnd, HDC& hdc, HGLRC& hglrc)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 8;
	pfd.cDepthBits = 16;
	hdc = GetDC(hWnd);

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (pixelFormat == 0)
	{
		return FALSE;
	}

	if (FALSE == SetPixelFormat(hdc, pixelFormat, &pfd))
	{
		return FALSE;
	}

	hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	return TRUE;
}

void app::winGlRelease(HWND hWnd, HDC& hdc, HGLRC& hglrc)
{
	wglMakeCurrent(NULL, NULL);

	if (hglrc != 0)
	{
		wglDeleteContext(hglrc);
	}

	hglrc = NULL;

	if (hdc != 0)
	{
		ReleaseDC(hWnd, hdc);
	}

	hdc = NULL;
}

LRESULT CALLBACK app::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

#else

app::app(unsigned int width, unsigned int height) : m_painter{width, height},
		m_dpy(nullptr),
		m_glc(nullptr),
		m_win(0)
{
	m_dpy = XOpenDisplay(NULL);

	if (m_dpy == NULL)
	{
		throw std::runtime_error("app::app cannot connect to X server");
	}

	Window root = DefaultRootWindow(m_dpy);
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo* vi = glXChooseVisual(m_dpy, 0, att);

	if (!vi)
	{
		throw std::runtime_error("app::app no appropriate visual found");
	}
	/*else
	{
		printf("\n\tvisual %p selected\n",
		       (void*)vi->visualid);  // %p creates hexadecimal output like in glxinfo
	}*/

	Colormap cmap = XCreateColormap(m_dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;
	m_win = XCreateWindow(m_dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput,
	                    vi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(m_dpy, m_win);
	XStoreName(m_dpy, m_win, "Title");
	m_glc = glXCreateContext(m_dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(m_dpy, m_win, m_glc);
	init();
}

app::~app()
{
	glXMakeCurrent(m_dpy, None, NULL);
	glXDestroyContext(m_dpy, m_glc);
	XDestroyWindow(m_dpy, m_win);
	XCloseDisplay(m_dpy);
}

int app::start()
{
	XEvent xev;
	XWindowAttributes gwa;
	do
	{
		XNextEvent(m_dpy, &xev);

		if (xev.type == Expose)
		{
			XGetWindowAttributes(m_dpy, m_win, &gwa);
			update();
			glXSwapBuffers(m_dpy, m_win);
		}
		else if (xev.type == KeyPress && xev.xkey.keycode == 0x09)
		{
			break;
		}
	} while(1);
	return 0;
}

#endif

void app::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
