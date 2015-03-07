#ifndef INTERNEL_RASTERIZED_COLOR_TRIANGLE_APP
#define INTERNEL_RASTERIZED_COLOR_TRIANGLE_APP

#include "internalApp.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"

using namespace lantern;

class internel_rasterized_color_triangle_app : public internalApp
{
public:
	internel_rasterized_color_triangle_app(unsigned int const aWidth, unsigned int const aHeight);
#if __ANDROID_API__ < 9
	int32_t on_key_down_for_jni(unsigned char const key) { return on_key_down(key); };
#endif
protected:
	void frame(float const delta_since_last_frame) override;
	int32_t on_key_down(unsigned char const key) override;

private:
	/** Updates view-model-projection matrix and gives it to the shader */
	void update_shader_mvp();

	vector3 const m_triangle_position;
	vector3 const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera m_camera;
	color_shader m_color_shader;
};

#endif
