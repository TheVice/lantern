#if !defined(__ANDROID__)
#include "app.h"

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

int main(int argc, char* argv[])
{
	return empty_app{640, 480}.start();
}

#else

#include "empty_app.h"

namespace lantern
{

empty_app::empty_app()
	: application()
{
}

#define colorInt2Float(value) (static_cast<float>(value) / 255)
void empty_app::frame(float dt)
{
	get_target_texture().clear(0);

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

	application::frame(dt);
}

}

#endif
