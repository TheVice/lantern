#include <iostream>
#include <stdexcept>
#if __linux
#include <unistd.h>
#endif
#include "app.h"

namespace lantern
{
#ifdef WIN32
#define WINDOW_CLASS TEXT("szWindowClass")

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

	app::app(unsigned int const width, unsigned int const height)
		: m_target_texture{width, height},
		  m_target_framerate_delay{0},
		  m_texName{0},
		  m_hwnd{HWND_DESKTOP},
		  m_hdc{0},
		  m_hglrc{0}
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

		initTexture(&m_texName);
		set_target_framerate(60);
		reshape(width, height);
	}

	app::~app()
	{
		// Release up OpenGL(WGL) library resources
		//
		releaseTexture(&m_texName);
		winGlRelease(m_hwnd, m_hdc, m_hglrc);
	}

	int app::start()
	{
		// Time when last frame was executed
		LARGE_INTEGER last_frame_time;
		QueryPerformanceCounter(&last_frame_time);

		double time_accumulator{0.0};
		unsigned int frames_accumulator{0};

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
				// Calculate time since last frame
				//
				LARGE_INTEGER current_time;
				QueryPerformanceCounter(&current_time);
				double delta_since_last_frame = ((double(
						current_time.LowPart - last_frame_time.LowPart)
						/ double(mFrequency.LowPart)) * 1000);

				// Save last frame time
				QueryPerformanceCounter(&last_frame_time);

				// Clear texture with black
				m_target_texture.clear(0);

				// Execute frame
				frame(delta_since_last_frame / 1000.0f);

				// Sum up passed time
				time_accumulator += delta_since_last_frame;

				// Present texture on a screen
				//
				dispalyTexture(m_texName, m_target_texture.get_width(), m_target_texture.get_height(), m_target_texture.get_data());
				SwapBuffers(m_hdc);

				// Sum up passed frames
				++frames_accumulator;

				if (m_target_framerate_delay > 0)
				{
					// Calculate delay we must have to stick to the target framerate
					//
					QueryPerformanceCounter(&current_time);
					double time_required_for_frame = ((double(
							current_time.LowPart - last_frame_time.LowPart)
							/ double(mFrequency.LowPart)) * 1000);
					int time_to_wait = m_target_framerate_delay - time_required_for_frame;

					if (time_to_wait > 0)
					{
						Sleep(time_to_wait);
					}
				}

				// Drop frames and seconds counters to zero every second
				//
				if (time_accumulator >= 1000.0)
				{
	#ifdef LANTERN_DEBUG_OUTPUT_FPS
					std::cout << "FPS: " << frames_accumulator << std::endl;
	#endif
					time_accumulator = 0;
					frames_accumulator = 0;
				}
			}
			// Process events
			//
			if (WM_CHAR == msg.message)
			{
				on_key_down((TCHAR) msg.wParam);
			}
		} while (WM_QUIT != msg.message);

		return 0;
	}
	#endif

	texture& app::get_target_texture()
	{
		return m_target_texture;
	}

	pipeline& app::get_pipeline()
	{
		return m_pipeline;
	}

	void app::set_target_framerate(unsigned int const fps)
	{
		if (fps == 0)
		{
			m_target_framerate_delay = 0;
		}
		else
		{
			m_target_framerate_delay = 1000 / fps;
		}
	}

	#if __linux
	unsigned long getTimer()
	{
		timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		return now.tv_nsec / 1000000 + now.tv_sec * 1000;
	}

	app::app(unsigned int const width, unsigned int const height) :
			m_target_texture{width, height},
			m_target_framerate_delay{0},
			m_texName{0},
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

		initTexture(&m_texName);
		set_target_framerate(60);
		reshape(width, height);
	}

	app::~app()
	{
		// Release up OpenGL(glx) library resources
		//
		releaseTexture(&m_texName);
		glXMakeCurrent(m_dpy, None, NULL);
		glXDestroyContext(m_dpy, m_glc);
		XDestroyWindow(m_dpy, m_win);
		XCloseDisplay(m_dpy);
	}

	int app::start()
	{
		// Time when last frame was executed
		unsigned long last_frame_time{getTimer()};
		unsigned long time_accumulator{0};
		unsigned int frames_accumulator{0};

		XEvent xev;
		XWindowAttributes gwa;
		do
		{
			XNextEvent(m_dpy, &xev);
			if (xev.type == Expose)
			{
				// Calculate time since last frame
				//
				unsigned long current_time = getTimer();
				unsigned long delta_since_last_frame{current_time - last_frame_time};

				// Save last frame time
				last_frame_time = getTimer();

				// Clear texture with black
				m_target_texture.clear(0);

				XGetWindowAttributes(m_dpy, m_win, &gwa);

				// Execute frame
				frame(delta_since_last_frame / 1000.0);

				// Sum up passed time
				time_accumulator += delta_since_last_frame;

				// Present texture on a screen
				//
				dispalyTexture(m_texName, m_target_texture.get_width(), m_target_texture.get_height(), m_target_texture.get_data());
				glXSwapBuffers(m_dpy, m_win);

				// Sum up passed frames
				++frames_accumulator;

				if (m_target_framerate_delay > 0)
				{
					// Calculate delay we must have to stick to the target framerate
					//
					uint32_t time_required_for_frame = getTimer() - last_frame_time;
					int time_to_wait = m_target_framerate_delay - time_required_for_frame;

					if (time_to_wait > 0)
					{
						usleep(static_cast<unsigned long>(time_to_wait / 1000.0));
					}
				}

				// Drop frames and seconds counters to zero every second
				//
				if (time_accumulator >= 1000)
				{
		#ifdef LANTERN_DEBUG_OUTPUT_FPS
					std::cout << "FPS: " << frames_accumulator << std::endl;
		#endif
					time_accumulator = 0;
					frames_accumulator = 0;
				}
				XSendEvent(m_dpy, m_win, 0, 0, &xev);
			}
			// Process events
			//
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
	#endif

	void app::reshape(GLsizei width, GLsizei height)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
	}

	void app::initTexture(GLuint* texName)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, texName);
		glBindTexture(GL_TEXTURE_2D, *texName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void app::dispalyTexture(GLuint texName, GLuint imageWidth, GLuint imageHeight, const GLubyte* image)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texName);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth,
				imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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
		glBindTexture(GL_TEXTURE_2D, 0);
		glFlush();
		glDisable(GL_TEXTURE_2D);
	}

	void app::releaseTexture(GLuint* texName)
	{
		glDeleteTextures(1, texName);
		(*texName) = 0;
	}
}
