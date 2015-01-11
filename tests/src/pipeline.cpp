#include "assert_utils.h"
#include "pipeline.h"

using namespace lantern;

class solid_color_shader final
{
public:
	solid_color_shader(color const& c) : m_color(c)
	{

	}

	std::vector<shader_bind_point_info<color>> get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{};
	}

	std::vector<shader_bind_point_info<float>> get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}
	std::vector<shader_bind_point_info<vector2f>> get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{};
	}

	std::vector<shader_bind_point_info<vector3>> get_vector3_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3>>{};
	}

	vector4 process_vertex(vector4 const& vertex) { return vertex; }
	color process_pixel(vector2ui const& pixel) { return m_color; }

private:
	color const m_color;
};

static void assert_pixel_centers_are_lit_no_ambiguities(pipeline& p)
{
	// Rasterizes triangle that doesn't have any pixel centers on edge
	// Picture: rasterization_no_ambiguities_test_case.png
	//

	texture texture{5, 5};
	solid_color_shader shader_white{color::WHITE};

	std::vector<vector3> triangle_vertices{
		vector3{0.05f, 0.9f, 0.0f}, vector3{-0.9f, 0.5f, 0.0f}, vector3{0.15, -0.8f, 0.0f}};
	std::vector<unsigned int> triangle_indices{0, 1, 2};
	mesh triangle_mesh{triangle_vertices, triangle_indices};
	texture.clear(0);
	p.draw(triangle_mesh, shader_white, texture);
	assert_pixels_two_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 1},
			vector2ui{1, 1}, vector2ui{1, 2},
			vector2ui{2, 0}, vector2ui{2, 1},  vector2ui{2, 2}, vector2ui{2, 3}},
		color::WHITE,
		color::BLACK);
}

static void assert_pixel_centers_are_lit_top_left_rule(pipeline& p)
{
	// Rasterizes triangle with pixel centers at the edges to check top-left filling rule
	//

	texture texture{5, 5};
	solid_color_shader shader_white{color::WHITE};

	// Picture: rasterization_top_left_rule_test_case_1.png
	//
	std::vector<vector3> triangle_vertices{
		vector3{-0.8f, 0.8f, 0.0f}, vector3{-0.8f, -0.8f, 0.0f}, vector3{0.8, 0.8f, 0.0f}};
	std::vector<unsigned int> triangle_indices{0, 1, 2};
	mesh triangle_mesh_1{triangle_vertices, triangle_indices};
	texture.clear(0);
	p.draw(triangle_mesh_1, shader_white, texture);
	assert_pixels_two_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 0}, vector2ui{0, 1}, vector2ui{0, 2}, vector2ui{0, 3},
			vector2ui{1, 0}, vector2ui{1, 1}, vector2ui{1, 2},
			vector2ui{2, 0}, vector2ui{2, 1},
			vector2ui{3, 0}},
		color::WHITE,
		color::BLACK);

	// Picture: rasterization_top_left_rule_test_case_2.png
	//
	triangle_vertices = std::vector<vector3>{
		vector3{0.8f, 0.8f, 0.0f}, vector3{-0.8f, 0.8f, 0.0f}, vector3{0.8, -0.8f, 0.0f}};
	triangle_indices = std::vector<unsigned int>{0, 1, 2};
	mesh triangle_mesh_2 = mesh{triangle_vertices, triangle_indices};
	texture.clear(0);
	p.draw(triangle_mesh_2, shader_white, texture);
	assert_pixels_two_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 0},
			vector2ui{1, 0}, vector2ui{1, 1},
			vector2ui{2, 0}, vector2ui{2, 1}, vector2ui{2, 2},
			vector2ui{3, 0}, vector2ui{3, 1}, vector2ui{3, 2}, vector2ui{3, 3}},
		color::WHITE,
		color::BLACK);

	// Picture: rasterization_top_left_rule_test_case_3.png
	//
	triangle_vertices = std::vector<vector3>{
		vector3{0.8f, 0.8f, 0.0f}, vector3{-0.8f, -0.8f, 0.0f}, vector3{0.8, -0.8f, 0.0f}};
	triangle_indices = std::vector<unsigned int>{0, 1, 2};
	mesh triangle_mesh_3 = mesh{triangle_vertices, triangle_indices};
	texture.clear(0);
	p.draw(triangle_mesh_3, shader_white, texture);
	assert_pixels_two_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{1, 3},
			vector2ui{2, 2}, vector2ui{2, 3},
			vector2ui{3, 1}, vector2ui{3, 2}, vector2ui{3, 3}},
		color::WHITE,
		color::BLACK);

	// Picture: rasterization_top_left_rule_test_case_4.png
	//
	triangle_vertices = std::vector<vector3>{
		vector3{-0.8f, 0.8f, 0.0f}, vector3{-0.8f, -0.8f, 0.0f}, vector3{0.8, -0.8f, 0.0f}};
	triangle_indices = std::vector<unsigned int>{0, 1, 2};
	mesh triangle_mesh_4 = mesh{triangle_vertices, triangle_indices};
	texture.clear(0);
	p.draw(triangle_mesh_4, shader_white, texture);
	assert_pixels_two_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 1}, vector2ui{0, 2}, vector2ui{0, 3},
			vector2ui{1, 2}, vector2ui{1, 3},
			vector2ui{2, 3}},
		color::WHITE,
		color::BLACK);

	solid_color_shader shader_blue{color::BLUE};

	// Picture: rasterization_top_left_rule_test_case_5.png
	//
	texture.clear(0);
	p.draw(triangle_mesh_4, shader_white, texture);
	p.draw(triangle_mesh_2, shader_blue, texture);
	assert_pixels_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 0},
			vector2ui{1, 0}, vector2ui{1, 1},
			vector2ui{2, 0}, vector2ui{2, 1}, vector2ui{2, 2},
			vector2ui{3, 0}, vector2ui{3, 1}, vector2ui{3, 2}, vector2ui{3, 3}},
		color::BLUE);
	assert_pixels_colors(
		texture,
		std::vector<vector2ui>{
			vector2ui{0, 1}, vector2ui{0, 2}, vector2ui{0, 3},
			vector2ui{1, 2}, vector2ui{1, 3},
			vector2ui{2, 3}},
		color::WHITE);
}

TEST(pipeline, mesh_rasterization_traversal_aabb)
{
	pipeline p;

	p.set_rasterization_algorithm(rasterization_algorithm_option::traversal_aabb);
	p.set_face_culling(face_culling_option::counter_clockwise);
	p.set_fill_mode(fill_mode_option::solid);
	assert_pixel_centers_are_lit_no_ambiguities(p);
	assert_pixel_centers_are_lit_top_left_rule(p);
}