#include "app.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"

using namespace lantern;

/** Draws simple triangle with interpolated color attribute */
#ifdef ANDROID

class rasterized_color_triangle_app : public app
{
public:
	rasterized_color_triangle_app(android_app* application);

protected:
	void frame(float const delta_since_last_frame) override;
	int32_t on_key_down(unsigned char const key) override;
	int32_t onActivate() override;
	void onDeactivate() override;

private:
	/** Updates view-model-projection matrix and gives it to the shader */
	void update_shader_mvp();

	vector3 const m_triangle_position;
	vector3 const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera* m_camera;
	color_shader m_color_shader;
};

#else

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

	vector3 const m_triangle_position;
	vector3 const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera m_camera;
	color_shader m_color_shader;
};

#endif

#ifdef ANDROID

rasterized_color_triangle_app::rasterized_color_triangle_app(android_app* application)
	: app(application),
	  m_triangle_position{0.0f, 0.0f, 1.5f},
	  m_triangle_rotation{0.0f, 0.0f, 0.0f},
	  m_triangle_mesh{load_mesh_from_obj("resources/triangle.obj", false, false)},
	  m_camera{nullptr}
{
	// Add color attribute to triangle mesh
	//
	std::vector<color> const colors{color::GREEN, color::RED, color::BLUE};
	std::vector<unsigned int> const indices{0, 1, 2};
	mesh_attribute_info<color> const color_info{VERTEX_COLOR_ATTR_ID, colors, indices};
	m_triangle_mesh.get_color_attributes().push_back(color_info);
}

#else

rasterized_color_triangle_app::rasterized_color_triangle_app(unsigned int const width, unsigned int const height)
	: app(width, height),
	  m_triangle_position{0.0f, 0.0f, 1.5f},
	  m_triangle_rotation{vector3::ZERO},
	  m_triangle_mesh{load_mesh_from_obj("resources/triangle.obj", false, false)},
	  m_camera{
		  vector3::ZERO,
		  vector3::Z_UNIT,
		  vector3::Y_UNIT,
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
		  0.01f,
		  20.0f}
{
	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	// Add color attribute to triangle mesh
	//
	std::vector<color> const colors{color::GREEN, color::RED, color::BLUE};
	std::vector<unsigned int> const indices{0, 1, 2};
	mesh_attribute_info<color> const color_info{VERTEX_COLOR_ATTR_ID, colors, indices};
	m_triangle_mesh.get_color_attributes().push_back(color_info);
};

#endif

void rasterized_color_triangle_app::frame(float const delta_since_last_frame)
{
	// Draw the triangle
	get_pipeline().draw(m_triangle_mesh, m_color_shader, get_target_texture());
}

#ifdef ANDROID

int32_t rasterized_color_triangle_app::on_key_down(unsigned char key)
{
	int32_t ret = 0;

	float const moving_speed{0.01f};

	if (key== AKEYCODE_VOLUME_UP)
	{
		m_camera->move_forward(moving_speed);
		ret = 1;
	}
	else if (key == AKEYCODE_VOLUME_DOWN)
	{
		m_camera->move_backward(moving_speed);
		ret = 1;
	}

	// Update model-view-projection according to camera changes
	update_shader_mvp();

	return ret;
}

#else

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

	// Update model-view-projection according to camera changes
	update_shader_mvp();
}

#endif

#ifdef ANDROID

int32_t rasterized_color_triangle_app::onActivate()
{
	app::onActivate();
	info("Activating rasterized_color_triangle_app OK");
	m_camera = new camera{vector3::ZERO,
						  vector3::Z_UNIT,
						  vector3::Y_UNIT,
						  static_cast<float>(M_PI) / 2.0f,
						  static_cast<float>(mWindowBuffer.height) / static_cast<float>(mWindowBuffer.width),
						  0.01f,
						  20.0f};

	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	return STATUS_OK;
}

void rasterized_color_triangle_app::onDeactivate()
{
	info("Deactivating rasterized_color_triangle_app");
	delete m_camera;
	m_camera = nullptr;
	info("rasterized_color_triangle_app deactivated");
	app::onDeactivate();
}

#endif

void rasterized_color_triangle_app::update_shader_mvp()
{
	matrix4x4 const local_to_world_transform{
		matrix4x4::rotation_around_x_axis(m_triangle_rotation.x) *
		matrix4x4::rotation_around_y_axis(m_triangle_rotation.y) *
		matrix4x4::rotation_around_z_axis(m_triangle_rotation.z) *
		matrix4x4::translation(m_triangle_position.x, m_triangle_position.y, m_triangle_position.z)};
#ifdef ANDROID
	matrix4x4 const camera_rotation{
		m_camera->get_right().x, m_camera->get_up().x, m_camera->get_forward().x, 0.0f,
		m_camera->get_right().y, m_camera->get_up().y, m_camera->get_forward().y, 0.0f,
		m_camera->get_right().z, m_camera->get_up().z, m_camera->get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
	matrix4x4 const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-m_camera->get_position().x, -m_camera->get_position().y, -m_camera->get_position().z, 1.0f};
#else
	matrix4x4 const camera_rotation{
		m_camera.get_right().x, m_camera.get_up().x, m_camera.get_forward().x, 0.0f,
		m_camera.get_right().y, m_camera.get_up().y, m_camera.get_forward().y, 0.0f,
		m_camera.get_right().z, m_camera.get_up().z, m_camera.get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};

	matrix4x4 const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-m_camera.get_position().x, -m_camera.get_position().y, -m_camera.get_position().z, 1.0f};
#endif
	matrix4x4 const world_to_camera_transform{camera_translation * camera_rotation};
#ifdef ANDROID
	matrix4x4 const camera_to_clip_transform{
		matrix4x4::clip_space(
			m_camera->get_horizontal_fov(),
			m_camera->get_vertical_fov(),
			m_camera->get_near_plane_z(),
			m_camera->get_far_plane_z())};
#else
	matrix4x4 const camera_to_clip_transform{
		matrix4x4::clip_space(
			m_camera.get_horizontal_fov(),
			m_camera.get_vertical_fov(),
			m_camera.get_near_plane_z(),
			m_camera.get_far_plane_z())};
#endif
	matrix4x4 const local_to_clip_transform{
		local_to_world_transform * world_to_camera_transform * camera_to_clip_transform};

	m_color_shader.set_mvp_matrix(local_to_clip_transform);
}

#ifdef ANDROID

void android_main(android_app* application)
{
	info("android_main");

	changeDirectoryToAppCacheLocation(application->activity->env, application->activity->vm,
	                                  application->activity->clazz);
	unpackResourcesFromApk(application->activity->assetManager);

	rasterized_color_triangle_app{application}.start();
}

#else

int main(int argc, char* argv[])
{
	return rasterized_color_triangle_app{640, 480}.start();
}

#endif
