#ifndef LANTERN_TEXTURE_H
#define LANTERN_TEXTURE_H

#include <cstring>
#include <cstddef>
#include "vector2.h"
#include "color.h"

namespace lantern
{
	/** Class representing ARGB8888 texture */
	class texture final
	{
	public:
		/** Constructs texture with given width and height
		* @param width Texture width
		* @param height Texture height
		*/
		texture(unsigned int const width, unsigned int const height);

		/** Frees memory used by the texture */
		~texture();

		/** Gets texture width
		* @returns Texture width
		*/
		unsigned int get_width() const;

		/** Gets texture height
		* @returns Texture height
		*/
		unsigned int get_height() const;

		/** Gets texture pitch (length of a row in bytes)
		* @returns Texture pitch
		*/
		unsigned int get_pitch() const;

		/** Gets texture raw data
		* @returns Texture raw data array
		*/
		unsigned char const* get_data() const;

		/** Gets pixel color at specified position
		* @param point Pixel coordinates to get color at
		* @returns Color at specified position
		*/
		color get_pixel_color(vector2ui const& point) const;

		/** Sets pixel color at specified position
		* @param point Pixel coordinates to set color at
		* @param color Color to set
		*/
		void set_pixel_color(vector2ui const& point, color const& color);

		/** Clears texture with specified byte value (thus clearing only with gray shade) */
		void clear(unsigned char const bytes_value);

	private:
		/** Texture width */
		unsigned int m_width;

		/** Texture height */
		unsigned int m_height;

		/** Size of texture data array in bytes */
		size_t m_data_total_size;

		/** Raw texture data */
		unsigned char* m_data;

		/** Texture pitch */
		unsigned int m_pitch;
	};

	inline void texture::set_pixel_color(vector2ui const& point, color const& color)
	{
		unsigned int const pixel_first_byte_index{m_pitch * point.y + point.x * 4};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		// m_data[pixel_first_byte_index + 0] is alpha, we don't use it for now
		m_data[pixel_first_byte_index + 1] = color.r;
		m_data[pixel_first_byte_index + 2] = color.g;
		m_data[pixel_first_byte_index + 3] = color.b;
#else
		m_data[pixel_first_byte_index + 0] = color.b;
		m_data[pixel_first_byte_index + 1] = color.g;
		m_data[pixel_first_byte_index + 2] = color.r;
		// m_data[pixel_first_byte_index + 3] is alpha, we don't use it for now
#endif
	}

	inline void texture::clear(unsigned char const bytes_value)
	{
		memset(m_data, bytes_value, m_data_total_size);
	}
}

#endif // LANTERN_TEXTURE_H