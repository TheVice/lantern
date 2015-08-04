#include <stdexcept>
#include <vector>
#include <fstream>
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

texture::texture(texture&& other)
	: m_width(other.m_width),
	  m_height(other.m_height),
	  m_data_total_size(other.m_data_total_size),
	  m_data{other.m_data},
	  m_pitch(other.m_pitch)
{
	other.m_data = nullptr;
}

texture::~texture()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
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

	return color{
		m_data[pixel_first_byte_index + 2] / 255.0f,
		m_data[pixel_first_byte_index + 1] / 255.0f,
		m_data[pixel_first_byte_index + 0] / 255.0f};
}

std::streamoff readFile(const char* aFileName,
						std::vector<char>& aFileContent)
{
	std::ifstream file(aFileName, std::fstream::binary);

	if (file)
	{
		file.seekg(0, std::fstream::end);
		const std::streamoff length = file.tellg();

		if (length == 0)
		{
			file.close();
			return 0;
		}

		file.seekg(0, std::fstream::beg);
		aFileContent.reserve(static_cast<std::string::size_type>(length));
		aFileContent.resize(static_cast<std::string::size_type>(length));
		file.read(&aFileContent[0], length);
		file.close();
		return length;
	}

	return 0;
}

extern int decodePNG(std::vector<unsigned char>& out_image,
                     unsigned long& image_width, unsigned long& image_height,
                     const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

texture texture::load_from_file(std::string file)
{
	std::vector<char> fileData;
	std::streamoff length = readFile(file.c_str(), fileData);
	if (length)
	{
		std::vector<unsigned char> png_image;
		unsigned long image_width;
		unsigned long image_height;
		decodePNG(png_image, image_width, image_height, reinterpret_cast<unsigned char*>(&fileData[0]), static_cast<unsigned int>(length));

		texture result(image_width, image_height);
		memcpy(result.m_data, &png_image[0], result.m_data_total_size);
		return result;
	}

	throw std::runtime_error{"load_from_file fail"};
}

