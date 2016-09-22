
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
}

void application::initialize(int width, int height)
{
	m_target_texture = std::make_unique<lantern::texture>(width, height);
	m_renderer = std::make_unique<lantern::renderer>();
}

void application::frame(float)
{
}

void application::on_key_down(unsigned char)
{
}

lantern::texture& application::get_target_texture()
{
	return *m_target_texture.get();
}

lantern::renderer& application::get_renderer()
{
	return *m_renderer.get();
}


}
