#ifdef ANDROID

#include <android/api-level.h>
#if __ANDROID_API__ < 9
enum {
    AKEYCODE_VOLUME_UP       = 24,
    AKEYCODE_VOLUME_DOWN     = 25
};
#endif
#include "internalApp.h"
#include "androidApp.h"
#include "misc.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"
#include "texture_shader.h"

using namespace lantern;

enum class shader_option
{
	color,
	texture
};

/** Draws simple triangle with interpolated color attribute */
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

	vector3f const m_triangle_position;
	vector3f const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera m_camera;

	color_shader m_color_shader;
	texture_shader m_texture_shader;
	shader_option m_shader_option;

	texture m_texture;
};
#if __ANDROID_API__ > 8
class android_rasterized_color_triangle_app : public androidApp
{
public:
	android_rasterized_color_triangle_app(android_app* aApplication) : androidApp(aApplication){};
protected:
	int32_t onActivate() override
	{
		androidApp::onActivate();
		mLanternApp = new internel_rasterized_color_triangle_app(mGraphics.getWidth(), mGraphics.getHeight());

		info("Activating App OK");
		return STATUS_OK;
	}
};
#endif
internel_rasterized_color_triangle_app::internel_rasterized_color_triangle_app(unsigned int const width, unsigned int const height)
	: internalApp(width, height),
	  m_triangle_position{0.0f, 0.0f, 1.5f},
	  m_triangle_rotation{vector3f{0.0f, 0.0f, 0.0f}},
	  m_triangle_mesh{load_mesh_from_obj("resources/triangle.obj", false, false)},
	  m_camera{
		  vector3f{0.0f, 0.0f, 0.0f},
		  vector3f{0.0f, 0.0f, 1.0f},
		  vector3f{0.0f, 1.0f, 0.0f},
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
		  0.01f,
		  20.0f},
	  m_shader_option{shader_option::color},
	  m_texture{texture::load_from_file("resources/chess.png")}
{
	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	std::vector<unsigned int> const indices{0, 1, 2};

	// Add color attribute to triangle mesh
	//
	std::vector<color> const colors{color::GREEN, color::RED, color::BLUE};
	mesh_attribute_info<color> const color_info{COLOR_ATTR_ID, colors, indices, attribute_interpolation_option::linear};
	m_triangle_mesh.get_color_attributes().push_back(color_info);

	// Add uv attribute to triangle mesh
	//
	std::vector<vector2f> uvs{vector2f{0.5f, 0.0f}, vector2f{0.0f, 1.0f}, vector2f{1.0f, 1.0f}};
	mesh_attribute_info<vector2f> uv_info{TEXCOORD_ATTR_ID, uvs, indices, attribute_interpolation_option::perspective_correct};
	m_triangle_mesh.get_vector2f_attributes().push_back(uv_info);

	// Setup texture shader
	//
	m_texture_shader.set_texture(&m_texture);
};

void internel_rasterized_color_triangle_app::frame(float const delta_since_last_frame)
{
	// Draw the triangle
	//

	if (m_shader_option == shader_option::color)
	{
		get_pipeline().draw(m_triangle_mesh, m_color_shader, get_target_texture());
	}
	else if (m_shader_option == shader_option::texture)
	{
		get_pipeline().draw(m_triangle_mesh, m_texture_shader, get_target_texture());
	}
}

int32_t internel_rasterized_color_triangle_app::on_key_down(unsigned char const key)
{
	int32_t ret = 0;

	if (key== AKEYCODE_VOLUME_UP)
	{
		m_shader_option = shader_option::color;
		ret = 1;
	}
	else if (key == AKEYCODE_VOLUME_DOWN)
	{
		m_shader_option = shader_option::texture;
		ret = 1;
	}

	// Update model-view-projection according to camera changes
	update_shader_mvp();

	return ret;
}

void internel_rasterized_color_triangle_app::update_shader_mvp()
{
	matrix4x4f const local_to_world_transform{
		matrix4x4f::rotation_around_x_axis(m_triangle_rotation.x) *
		matrix4x4f::rotation_around_y_axis(m_triangle_rotation.y) *
		matrix4x4f::rotation_around_z_axis(m_triangle_rotation.z) *
		matrix4x4f::translation(m_triangle_position.x, m_triangle_position.y, m_triangle_position.z)};

	matrix4x4f const camera_rotation{
		m_camera.get_right().x, m_camera.get_up().x, m_camera.get_forward().x, 0.0f,
		m_camera.get_right().y, m_camera.get_up().y, m_camera.get_forward().y, 0.0f,
		m_camera.get_right().z, m_camera.get_up().z, m_camera.get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};

	matrix4x4f const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-m_camera.get_position().x, -m_camera.get_position().y, -m_camera.get_position().z, 1.0f};

	matrix4x4f const world_to_camera_transform{camera_translation * camera_rotation};

	matrix4x4f const camera_to_clip_transform{
		matrix4x4f::clip_space(
			m_camera.get_horizontal_fov(),
			m_camera.get_vertical_fov(),
			m_camera.get_near_plane_z(),
			m_camera.get_far_plane_z())};

	matrix4x4f const local_to_clip_transform{
		local_to_world_transform * world_to_camera_transform * camera_to_clip_transform};

	if (m_shader_option == shader_option::color)
	{
		m_color_shader.set_mvp_matrix(local_to_clip_transform);
	}
	else
	{
		m_texture_shader.set_mvp_matrix(local_to_clip_transform);
	}
}
#if __ANDROID_API__ > 8
void android_main(android_app* application)
{
	info("android_main");

	changeDirectoryToAppCacheLocation(application->activity->env, application->activity->vm,
	                                  application->activity->clazz);
	unpackResourcesFromApk(application->activity->assetManager);

	android_rasterized_color_triangle_app{application}.start();
}
#endif
#else

#include "app.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"
#include "texture_shader.h"

using namespace lantern;

enum class shader_option
{
	color,
	texture
};

/** Draws simple triangle with interpolated color attribute */
class rasterized_color_triangle_app : public app
{
public:
	rasterized_color_triangle_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float const delta_since_last_frame) override;
	void on_key_down(SDL_Keysym const key) override;

private:
	/** Updates view-model-projection matrix and gives it to the shader */
	void update_shader_mvp();

	vector3f const m_triangle_position;
	vector3f const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera m_camera;
	
	color_shader m_color_shader;
	texture_shader m_texture_shader;
	shader_option m_shader_option;

	texture m_texture;
};

rasterized_color_triangle_app::rasterized_color_triangle_app(unsigned int const width, unsigned int const height)
	: app(width, height),
	  m_triangle_position{0.0f, 0.0f, 1.5f},
	  m_triangle_rotation{vector3f{0.0f, 0.0f, 0.0f}},
	  m_triangle_mesh{load_mesh_from_obj("resources/triangle.obj", false, false)},
	  m_camera{
		  vector3f{0.0f, 0.0f, 0.0f},
		  vector3f{0.0f, 0.0f, 1.0f},
		  vector3f{0.0f, 1.0f, 0.0f},
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
		  0.01f,
		  20.0f},
	  m_shader_option{shader_option::color},
	  m_texture{texture::load_from_file("resources/chess.png")}
{
	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	std::vector<unsigned int> const indices{0, 1, 2};

	// Add color attribute to triangle mesh
	//
	std::vector<color> const colors{color::GREEN, color::RED, color::BLUE};
	mesh_attribute_info<color> const color_info{COLOR_ATTR_ID, colors, indices, attribute_interpolation_option::linear};
	m_triangle_mesh.get_color_attributes().push_back(color_info);

	// Add uv attribute to triangle mesh
	//
	std::vector<vector2f> uvs{vector2f{0.5f, 0.0f}, vector2f{0.0f, 1.0f}, vector2f{1.0f, 1.0f}};
	mesh_attribute_info<vector2f> uv_info{TEXCOORD_ATTR_ID, uvs, indices, attribute_interpolation_option::perspective_correct};
	m_triangle_mesh.get_vector2f_attributes().push_back(uv_info);

	// Setup texture shader
	//
	m_texture_shader.set_texture(&m_texture);
};

void rasterized_color_triangle_app::frame(float const delta_since_last_frame)
{
	// Draw the triangle
	//

	if (m_shader_option == shader_option::color)
	{
		get_pipeline().draw(m_triangle_mesh, m_color_shader, get_target_texture());
	}
	else if (m_shader_option == shader_option::texture)
	{
		get_pipeline().draw(m_triangle_mesh, m_texture_shader, get_target_texture());
	}
}

void rasterized_color_triangle_app::on_key_down(SDL_Keysym const key)
{
	app::on_key_down(key);

	float const moving_speed{0.01f};
	float const rotation_speed{0.05f};

	if (key.sym == SDLK_a)
	{
		m_camera.move_left(moving_speed);
	}
	else if (key.sym == SDLK_d)
	{
		m_camera.move_right(moving_speed);
	}
	else if (key.sym == SDLK_w)
	{
		m_camera.move_forward(moving_speed);
	}
	else if (key.sym == SDLK_s)
	{
		m_camera.move_backward(moving_speed);
	}
	else if (key.sym == SDLK_r)
	{
		m_camera.move_up(moving_speed);
	}
	else if (key.sym == SDLK_f)
	{
		m_camera.move_down(moving_speed);
	}
	else if (key.sym == SDLK_q)
	{
		m_camera.yaw(-rotation_speed);
	}
	else if (key.sym == SDLK_e)
	{
		m_camera.yaw(rotation_speed);
	}
	else if (key.sym == SDLK_1)
	{
		m_shader_option = shader_option::color;
	}
	else if (key.sym == SDLK_2)
	{
		m_shader_option = shader_option::texture;
	}

	// Update model-view-projection according to camera changes
	update_shader_mvp();
}

void rasterized_color_triangle_app::update_shader_mvp()
{
	matrix4x4f const local_to_world_transform{
		matrix4x4f::rotation_around_x_axis(m_triangle_rotation.x) *
		matrix4x4f::rotation_around_y_axis(m_triangle_rotation.y) *
		matrix4x4f::rotation_around_z_axis(m_triangle_rotation.z) *
		matrix4x4f::translation(m_triangle_position.x, m_triangle_position.y, m_triangle_position.z)};

	matrix4x4f const camera_rotation{
		m_camera.get_right().x, m_camera.get_up().x, m_camera.get_forward().x, 0.0f,
		m_camera.get_right().y, m_camera.get_up().y, m_camera.get_forward().y, 0.0f,
		m_camera.get_right().z, m_camera.get_up().z, m_camera.get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};

	matrix4x4f const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-m_camera.get_position().x, -m_camera.get_position().y, -m_camera.get_position().z, 1.0f};

	matrix4x4f const world_to_camera_transform{camera_translation * camera_rotation};

	matrix4x4f const camera_to_clip_transform{
		matrix4x4f::clip_space(
			m_camera.get_horizontal_fov(),
			m_camera.get_vertical_fov(),
			m_camera.get_near_plane_z(),
			m_camera.get_far_plane_z())};

	matrix4x4f const local_to_clip_transform{
		local_to_world_transform * world_to_camera_transform * camera_to_clip_transform};

	if (m_shader_option == shader_option::color)
	{
		m_color_shader.set_mvp_matrix(local_to_clip_transform);
	}
	else
	{
		m_texture_shader.set_mvp_matrix(local_to_clip_transform);
	}
}

int main(int argc, char* argv[])
{
	return rasterized_color_triangle_app{640, 480}.start();
}

#endif
