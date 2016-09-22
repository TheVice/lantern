
#include "win32_application.h"

#if defined(WIN32)

namespace application
{

win32_application::win32_application(HINSTANCE hinstance) :
	application(),
	mWindow(),
	mWindowHandle(HWND_DESKTOP)
{
	mWindow.hInstance = hinstance;
}

win32_application::~win32_application()
{
	UnregisterClass(mWindow.lpszClassName, mWindow.hInstance);
}

void win32_application::initialize(int width, int height)
{
	application::initialize(width, height);
	//
	sHandlers.push_back(MessageHandler(WM_DESTROY, [](HWND, WPARAM, LPARAM) -> LRESULT
	{
		PostQuitMessage(0);
		return 0;
	}));
	//
	mWindow.cbSize = sizeof(mWindow);
	mWindow.style = CS_CLASSDC;
	//
	mWindow.lpfnWndProc = [](HWND aWindow, UINT aMessage, WPARAM aWparam, LPARAM aLparam) -> LRESULT
	{
		for (const auto& h : sHandlers)
		{
			if (aMessage == h.mMessage)
			{
				return h.mHandler(aWindow, aWparam, aLparam);
			}
		}

		return DefWindowProc(aWindow, aMessage, aWparam, aLparam);
	};
	//
	mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	mWindow.lpszClassName = TEXT("win32_application_class");
	//
	RegisterClassEx(&mWindow);
	//
	RECT windowRectangle = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);
	//
	POINT center = CenterWindow(width, height);
	mWindowHandle = CreateWindow(mWindow.lpszClassName, TEXT(""), WS_OVERLAPPEDWINDOW,
								 center.x, center.y,
								 windowRectangle.right - windowRectangle.left,
								 windowRectangle.bottom - windowRectangle.top,
								 HWND_DESKTOP, nullptr, mWindow.hInstance, nullptr);
	ShowWindow(mWindowHandle, SW_SHOW);
	UpdateWindow(mWindowHandle);
}

POINT win32_application::CenterWindow(int aWindowWidth, int aWindowHeight)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	POINT center;
	center.x = static_cast<LONG>(static_cast<float>(screenWidth - aWindowWidth) / 2);
	center.y = static_cast<LONG>(static_cast<float>(screenHeight - aWindowHeight) / 2);
	return center;
}

std::vector<MessageHandler> win32_application::sHandlers;

}

#endif
