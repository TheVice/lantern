#ifndef LANTERN_MERGER_H
#define LANTERN_MERGER_H

#include "texture.h"
#include "vector2.h"

namespace lantern
{
	class merger final
	{
	public:
		merger();

		void merge(texture& target_texture, vector2ui const& pixel_coordinates, color const& pixel_color) const;

		void set_alpha_blending_enabled(bool enabled);

	private:
		/** True = using colors alpha components during merge */
		bool m_alpha_blending_enabled;
	};
}

#endif