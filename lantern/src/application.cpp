
#include "application.h"

namespace application
{

application::application() :
	m_target_texture(nullptr),
	m_renderer(nullptr)
{
}

application::~application()
{
	if (m_renderer)
	{
		delete m_renderer;
	}

	if (m_target_texture)
	{
		delete m_target_texture;
	}
}

void application::initialize(int width, int height)
{
	if (m_renderer)
	{
		delete m_renderer;
	}

	if (m_target_texture)
	{
		delete m_target_texture;
	}

	m_target_texture = new lantern::texture(width, height);
	m_renderer = new lantern::renderer();
}

void application::frame(float)
{
}

void application::on_key_down(unsigned char)
{
}

lantern::texture& application::get_target_texture()
{
	return *m_target_texture;
}

lantern::renderer& application::get_renderer()
{
	return *m_renderer;
}


}
