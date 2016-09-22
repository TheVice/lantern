#ifndef _WIN32_APPLICATION_H_
#define _WIN32_APPLICATION_H_

#include "application.h"
#if defined(WIN32)
#include <windows.h>

namespace application
{

//typedef LRESULT(*MessageCallback)(HWND, WPARAM, LPARAM);
using MessageCallback = LRESULT(*)(HWND, WPARAM, LPARAM);

struct MessageHandler
{
	UINT mMessage;
	MessageCallback mHandler;
	MessageHandler(UINT aMessage, MessageCallback aHandler) : mMessage(aMessage),
		mHandler(aHandler)
	{
	}
};

class win32_application : public application
{
public:
	win32_application(HINSTANCE hinstance);
	virtual ~win32_application();

	virtual void initialize(int width, int height) override;

public:
	win32_application() = delete;
	win32_application(const win32_application& aRhs) = delete;
	win32_application& operator = (const win32_application& aRhs) = delete;

public:
	static POINT CenterWindow(int aWindowWidth, int aWindowHeight);

protected:
	WNDCLASSEX mWindow;
	HWND mWindowHandle;

protected:
	static std::vector<MessageHandler> sHandlers;
};

}

#endif
#endif
