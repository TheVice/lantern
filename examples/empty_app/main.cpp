#include "app.h"
#if defined(__ANDROID__)
#include <jni.h>
#endif

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
#define colorInt2Float(value) ((value) * 1.0f / 255.0f)
void empty_app::frame(float delta_since_last_frame)
{
	const int barSize = get_target_texture().get_width() / 8;
	for (int j = 0; j < get_target_texture().get_height(); ++j)
	{
		for (int i = 0; i < barSize; ++i)
		{
			get_target_texture().set_pixel_color(vector2ui{i, j},
												 color{colorInt2Float(255), colorInt2Float(255), colorInt2Float(255), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{barSize + i, j},
												 color{colorInt2Float(196), colorInt2Float(196), colorInt2Float(0), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{2 * barSize + i, j},
												 color{colorInt2Float(0), colorInt2Float(196), colorInt2Float(196), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{3 * barSize + i, j},
												 color{colorInt2Float(0), colorInt2Float(196), colorInt2Float(0), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{4 * barSize + i, j},
												 color{colorInt2Float(196), colorInt2Float(0), colorInt2Float(196), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{5 * barSize + i, j},
												 color{colorInt2Float(196), colorInt2Float(0), colorInt2Float(0), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{6 * barSize + i, j},
												 color{colorInt2Float(0), colorInt2Float(0), colorInt2Float(196), colorInt2Float(255)});
			get_target_texture().set_pixel_color(vector2ui{7 * barSize + i, j},
												 color{colorInt2Float(0), colorInt2Float(0), colorInt2Float(0), colorInt2Float(255)});
		}
	}
}
#if !defined(__ANDROID__)
int main(int argc, char* argv[])
{
	return empty_app{640, 480}.start();
}
#else
extern "C" {
JNIEXPORT void JNICALL Java_org_lantern_EmptyAppActivity_getImage4Activity(JNIEnv *env, jclass cls,
		jintArray colors,
int width, int height)
{
jint *elements = env->GetIntArrayElements(colors, 0);

empty_app{width, height}.start(elements);

env->SetIntArrayRegion(colors, 0, width * height, elements);
env->ReleaseIntArrayElements(colors, elements, 0);
}
}
#endif
