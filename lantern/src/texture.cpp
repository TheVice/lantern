#include "texture.h"

using namespace lantern;

texture::texture(unsigned int const width, unsigned int const height)
	: m_width{width},
	  m_height{height},
	  m_data_total_size{width * height * 4},
	  m_data{new unsigned char[m_data_total_size]},
	  m_pitch{width * 4}
{
}

texture::~texture()
{
	delete[] m_data;
	m_data = nullptr;
}

unsigned int texture::get_width() const
{
	return m_width;
}

unsigned int texture::get_height() const
{
	return m_height;
}

unsigned int texture::get_pitch() const
{
	return m_pitch;
}

unsigned char const* texture::get_data() const
{
	return m_data;
}

color texture::get_pixel_color(vector2ui const& point) const
{
	unsigned int const pixel_first_byte_index{m_pitch * point.y + point.x * 4};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return color{
		m_data[pixel_first_byte_index + 1],
		m_data[pixel_first_byte_index + 2],
		m_data[pixel_first_byte_index + 3]};
#else
	return color{
		m_data[pixel_first_byte_index + 0],
		m_data[pixel_first_byte_index + 1],
		m_data[pixel_first_byte_index + 2]};
#endif
}
