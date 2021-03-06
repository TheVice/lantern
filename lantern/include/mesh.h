#ifndef LANTERN_MESH_H
#define LANTERN_MESH_H

#include <vector>
#include "mesh_attribute_info.h"
#include "vector2.h"
#include "vector3.h"
#include "color.h"

namespace lantern
{
	/** Class representing mesh.
	* It can be viewed as a container that holds vertices, attributes and their indices
	*/
	class mesh final
	{
	public:
		/** Constructs empy mesh */
		mesh();

		/** Constructs mesh with specified vertices and indices
		* @param vertices Mesh vertices
		* @param indices Mesh indices
		*/
		mesh(std::vector<vector3f> vertices, std::vector<unsigned int> indices);

		/** Gets mesh vertices
		* @returns Mesh vertices
		*/
		std::vector<vector3f> const& get_vertices() const;

		/** Gets mesh vertices
		* @returns Mesh vertices
		*/
		std::vector<vector3f>& get_vertices();

		/** Get mesh indices
		* @returns Mesh indices
		*/
		std::vector<unsigned int> const& get_indices() const;

		/** Get mesh indices
		* @returns Mesh indices
		*/
		std::vector<unsigned int>& get_indices();

		/** Gets color attributes
		* @returns Color attributes storage
		*/
		std::vector<mesh_attribute_info<color>>& get_color_attributes();

		/** Gets color attributes
		* @returns Color attributes const storage
		*/
		std::vector<mesh_attribute_info<color>> const& get_color_attributes() const;

		/** Gets float attributes
		* @returns float attributes storage
		*/
		std::vector<mesh_attribute_info<float>>& get_float_attributes();

		/** Gets float attributes
		* @returns float attributes const storage
		*/
		std::vector<mesh_attribute_info<float>> const& get_float_attributes() const;

		/** Gets vector2f attributes
		* @returns vector2f attributes storage
		*/
		std::vector<mesh_attribute_info<vector2f>>& get_vector2f_attributes();

		/** Gets vector2f attributes
		* @returns vector2f attributes const storage
		*/
		std::vector<mesh_attribute_info<vector2f>> const& get_vector2f_attributes() const;

		/** Gets vector3f attributes
		* @returns vector3f attributes storage
		*/
		std::vector<mesh_attribute_info<vector3f>>& get_vector3f_attributes();

		/** Gets vector3f attributes
		* @returns vector3f attributes const storage
		*/
		std::vector<mesh_attribute_info<vector3f>> const& get_vector3f_attributes() const;

	private:
		/** Mesh vertices */
		std::vector<vector3f> m_vertices;

		/** Mesh indices */
		std::vector<unsigned int> m_indices;

		/** Mesh color attributes */
		std::vector<mesh_attribute_info<color>> m_color_attributes;

		/** Mesh float attributes */
		std::vector<mesh_attribute_info<float>> m_float_attributes;

		/** Mesh vector2f attributes */
		std::vector<mesh_attribute_info<vector2f>> m_vector2f_attributes;

		/** Mesh vector3f attributes */
		std::vector<mesh_attribute_info<vector3f>> m_vector3f_attributes;
	};
}

#endif // LANTERN_MESH_H
