#ifndef _GDI_APPLICATION_H_
#define _GDI_APPLICATION_H_

#include "win32_application.h"
#if defined(WIN32)

namespace application
{

class gdi_application : public win32_application
{
public:
	gdi_application(HINSTANCE hinstance);
	virtual ~gdi_application();

	virtual void initialize(int width, int height) override;
	virtual void frame(float dt) override;

public:
	gdi_application() = delete;
	gdi_application(const gdi_application& aRhs) = delete;
	gdi_application& operator = (const gdi_application& aRhs) = delete;

private:
	HBITMAP mBitmap;
};

}

#endif
#endif
