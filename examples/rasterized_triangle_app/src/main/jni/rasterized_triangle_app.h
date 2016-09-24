#ifndef _RASTERIZED_TRIANGLE_APP_H_
#define _RASTERIZED_TRIANGLE_APP_H_

#include "application.h"
#include "camera.h"
#include "color_shader.h"
#include <map>

namespace lantern
{

class rasterized_triangle_app : public application::application
{
public:
	rasterized_triangle_app();
	virtual ~rasterized_triangle_app() {};

public:
	virtual void initialize(int width, int height) override;
	virtual void frame(float dt) override;
	virtual void on_key_down(unsigned char key) override;

private:
	/** Updates view-model-projection matrix and gives it to the shader */
	void update_shader_mvp();

	vector3f const m_triangle_position;
	vector3f const m_triangle_rotation;
	std::unique_ptr<mesh> m_triangle_mesh;

	std::unique_ptr<camera> m_camera;

	color_shader m_color_shader;

public:
	static std::map<std::string, std::string> mResources;
};

}

#endif
