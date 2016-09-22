
#include "gdi_application.h"

#if defined(WIN32)

namespace application
{

gdi_application::gdi_application(HINSTANCE hinstance) :
	win32_application(hinstance),
	mBitmap(nullptr)
{
}

gdi_application::~gdi_application()
{
	DeleteObject(mBitmap);
}

void gdi_application::initialize(int width, int height)
{
	win32_application::initialize(width, height);
	//
	HDC hdc = GetDC(mWindowHandle);
	mBitmap = CreateCompatibleBitmap(hdc, width, height);
	DeleteDC(hdc);
}

void gdi_application::frame(float dt)
{
	BITMAP bm;
	//
	HDC hdc = GetDC(mWindowHandle);
	//
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, mBitmap);
	//
	GetObject(mBitmap, sizeof(bm), &bm);
	//
	SetBitmapBits(mBitmap, bm.bmWidthBytes * bm.bmHeight, get_target_texture().get_data());
	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	//
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	//
	DeleteDC(hdc);
	//
	win32_application::frame(dt);
}

}

#endif
