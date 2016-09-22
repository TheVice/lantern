#include "app.h"
#include "gdi_application.h"

using namespace lantern;

class empty_app : public app
{
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float const delta_since_last_frame) override;
};

empty_app::empty_app(unsigned int const width, unsigned int const height)
	: app(width, height)
{
}

void empty_app::frame(float delta_since_last_frame)
{
}
#if defined(WIN32)
class empty_application : public application::gdi_application
{
public:
	empty_application();
	virtual ~empty_application();

	void frame(float dt) override;
};

empty_application::empty_application() :
	gdi_application::gdi_application(GetModuleHandle(nullptr))
{
}

empty_application::~empty_application()
{
}
#define colorInt2Float(value) (static_cast<float>(value) / 255)
void empty_application::frame(float dt)
{
	const auto barSize = get_target_texture().get_width() / 8;

	for (auto j = 0u; j < get_target_texture().get_height(); ++j)
	{
		for (auto i = 0u; i < barSize; ++i)
		{
			get_target_texture().set_pixel_color(vector2ui{ i, j },
												 color{ colorInt2Float(255), colorInt2Float(255), colorInt2Float(255), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ barSize + i, j },
												 color{ colorInt2Float(196), colorInt2Float(196), colorInt2Float(0), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 2 * barSize + i, j },
												 color{ colorInt2Float(0), colorInt2Float(196), colorInt2Float(196), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 3 * barSize + i, j },
												 color{ colorInt2Float(0), colorInt2Float(196), colorInt2Float(0), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 4 * barSize + i, j },
												 color{ colorInt2Float(196), colorInt2Float(0), colorInt2Float(196), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 5 * barSize + i, j },
												 color{ colorInt2Float(196), colorInt2Float(0), colorInt2Float(0), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 6 * barSize + i, j },
												 color{ colorInt2Float(0), colorInt2Float(0), colorInt2Float(196), colorInt2Float(255) });
			get_target_texture().set_pixel_color(vector2ui{ 7 * barSize + i, j },
												 color{ colorInt2Float(0), colorInt2Float(0), colorInt2Float(0), colorInt2Float(255) });
		}
	}

	gdi_application::frame(dt);
}
#endif
int main(int argc, char* argv[])
{
#if !defined(WIN32)
	return empty_app {640, 480} .start();
#else
	empty_application appication;
	appication.initialize(640, 480);
	MSG msg = { 0 };

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			appication.frame(0.0f);
		}
	}
	while (WM_QUIT != msg.message);

	return 0;
#endif
}
