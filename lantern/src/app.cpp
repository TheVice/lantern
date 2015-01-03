#include <iostream>
#include <stdexcept>

#include "app.h"

using namespace lantern;

#ifdef WIN32
#define WINDOW_CLASS TEXT("szWindowClass")
app::app(unsigned int const width, unsigned int const height)
	: m_painter{width, height},
      m_texName(0),
      m_hwnd(HWND_DESKTOP),
      m_hdc(0),
      m_hglrc(0)
{
	QueryPerformanceFrequency(&mFrequency);
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
	reshape(width, height);
}

app::~app()
{
	winGlRelease(m_hwnd, m_hdc, m_hglrc);
}

int app::start()
{
	LARGE_INTEGER timerStart;
	QueryPerformanceCounter(&timerStart);
	double time_accumulator_millis = 0.0;
	unsigned int fps = 0;
	MSG msg;
	do
	{
		// Process events
		//
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Clear texture
			m_painter.clear(0);

			// Calculate time since last frame
			//
			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);
			double delta_since_last_frame_millis = ((double(
					currentTime.LowPart - timerStart.LowPart)
					/ double(mFrequency.LowPart)) * 1000);

			// Save last frame time
			QueryPerformanceCounter(&timerStart);

			// Execute frame
			frame(delta_since_last_frame_millis / 1000.0);

			// Sum up passed time
			time_accumulator_millis += delta_since_last_frame_millis;

			// Present texture
			initTexture(&m_texName, m_painter.get_bitmap_width(), m_painter.get_bitmap_height(), m_painter.get_data());
			dispalyTexture(m_texName);
			SwapBuffers(m_hdc);

			++fps;

			if (time_accumulator_millis > 1000.0)
			{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
				std::cout << "FPS: " << fps << std::endl;
#endif
				time_accumulator_millis = 0.0;
				fps = 0;
			}
		}
		if (WM_CHAR == msg.message)
		{
			on_key_down((TCHAR) msg.wParam);
		}
	} while (WM_QUIT != msg.message);

	return 0;
}

ATOM app::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WINDOW_CLASS;
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

	aHwnd = CreateWindowEx(dwExStyle, WINDOW_CLASS, TEXT("Title"), dwStyle,
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

	hglrc = 0;

	if (hdc != 0)
	{
		ReleaseDC(hWnd, hdc);
	}

	hdc = 0;
}

LRESULT CALLBACK app::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			reshape(LOWORD(lParam), HIWORD(lParam));
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

#else

app::app(unsigned int const width, unsigned int const height) : m_painter{width, height},
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
	reshape(width, height);
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
	unsigned long timerStart = getTimer();
	unsigned long time_accumulator_millis = 0;
	unsigned int fps = 0;
	XEvent xev;
	XWindowAttributes gwa;
	do
	{
		XNextEvent(m_dpy, &xev);
		if (xev.type == Expose)
		{
			// Clear texture
			m_painter.clear(0);

			// Calculate time since last frame
			//
			unsigned long currentTime = getTimer();
			unsigned long delta_since_last_frame_millis = currentTime - timerStart;

			// Save last frame time
			timerStart = getTimer();

			XGetWindowAttributes(m_dpy, m_win, &gwa);

			// Execute frame
			frame(delta_since_last_frame_millis / 1000.0);

			// Sum up passed time
			time_accumulator_millis += delta_since_last_frame_millis;

			// Present texture
			initTexture(&m_texName, m_painter.get_bitmap_width(), m_painter.get_bitmap_height(), m_painter.get_data());
			dispalyTexture(m_texName);
			glXSwapBuffers(m_dpy, m_win);

			++fps;

			if (time_accumulator_millis >= 1000)
			{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
				std::cout << "FPS: " << fps << std::endl;
#endif
				time_accumulator_millis = 0;
				fps = 0;
			}
			XSendEvent(m_dpy, m_win, 0, 0, &xev);
		}
		else if (xev.type == KeyPress)
		{
			if (xev.xkey.keycode == 0x09)
			{
				break;
			}
			on_key_down(xev.xkey.keycode);
		}
	} while(1);
	return 0;
}

unsigned long app::getTimer()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_nsec / 1000000 + now.tv_sec * 1000;
}
#endif

bitmap_painter& app::get_painter()
{
	return m_painter;
}

void app::reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 30.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(0.0, 0.0, -3.0);
}

void app::initTexture(GLuint* texName, GLuint imageWidth, GLuint imageHeight, const GLubyte* image)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, texName);
	glBindTexture(GL_TEXTURE_2D, *texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth,
			imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void app::dispalyTexture(GLuint texName)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, -1.0, 0.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, -1.0, 0.0);

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}
