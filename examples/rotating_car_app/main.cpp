#include "app.h"
#include "scene.h"
#include "obj_import.h"
#include "pipeline.h"

using namespace lantern;

class rotating_car_app : public app
{
public:
	rotating_car_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float delta_since_last_frame) override;
	void on_key_down(unsigned char key) override;

private:
	scene m_scene;
	camera m_camera;
	pipeline m_pipeline;
};

rotating_car_app::rotating_car_app(unsigned int const width, unsigned int const height)
	: app(width, height),
	  m_camera{
		  vector3::ZERO,
		  vector3{0.0f, 0.0f, 1.0f},
		  vector3::Y_UNIT,
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
	  	  0.01f,
	  	  20.0f}
{
	// Load mesh and initialize scene
	//

	scene_object car_object{import_obj_mesh("resources/car.obj")};
	car_object.set_world_position(vector3{0.0f, 0.0f, 5.0f});
	car_object.set_world_rotation(vector3{static_cast<float>(M_PI) / 8.0f, static_cast<float>(M_PI), 0.0f});
	m_scene.get_objects().push_back(car_object);
}

void rotating_car_app::frame(float delta_since_last_frame)
{
	// Rotate by 45 degrees per second
	m_scene.get_objects()[0].get_world_rotation().y += static_cast<float>(M_PI) / 4.0f * delta_since_last_frame;

	// Draw
	m_pipeline.draw_scene(m_scene, m_camera, get_painter());
}

void rotating_car_app::on_key_down(unsigned char key)
{
	float const moving_speed = 0.1f;
	float const rotation_speed = 0.05f;
#ifdef WIN32
	if (key == 'a')
	{
		m_camera.move_left(moving_speed);
	}
	else if (key == 'd')
	{
		m_camera.move_right(moving_speed);
	}
	else if (key == 'w')
	{
		m_camera.move_forward(moving_speed);
	}
	else if (key == 's')
	{
		m_camera.move_backward(moving_speed);
	}
	else if (key == 'r')
	{
		m_camera.move_up(moving_speed);
	}
	else if (key == 'f')
	{
		m_camera.move_down(moving_speed);
	}
	else if (key == 'q')
	{
		m_camera.yaw(-rotation_speed);
	}
	else if (key == 'e')
	{
		m_camera.yaw(rotation_speed);
	}
#else
	if (key == 0x26)
	{
		m_camera.move_left(moving_speed);
	}
	else if (key == 0x28)
	{
		m_camera.move_right(moving_speed);
	}
	else if (key == 0x19)
	{
		m_camera.move_forward(moving_speed);
	}
	else if (key == 0x27)
	{
		m_camera.move_backward(moving_speed);
	}
	else if (key == 0x1b)
	{
		m_camera.move_up(moving_speed);
	}
	else if (key == 0x29)
	{
		m_camera.move_down(moving_speed);
	}
	else if (key == 0x18)
	{
		m_camera.yaw(-rotation_speed);
	}
	else if (key == 0x1a)
	{
		m_camera.yaw(rotation_speed);
	}
#endif
}

int main(int argc, char* argv[])
{
	return rotating_car_app{640, 480}.start();
}