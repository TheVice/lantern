#include "assert_utils.h"
#include "matrix3x3.h"

using namespace lantern;

TEST(matrix3x3f, constructors)
{
	matrix3x3f const m1{
		15.0f, -3.0f, 20.0f,
		13.1f, -1.1f, 17.1f,
		90.2f, 1.2f, -2.2f};
	assert_floats_near(m1.values[0][0], 15.0f);
	assert_floats_near(m1.values[0][1], -3.0f);
	assert_floats_near(m1.values[0][2], 20.0f);
	assert_floats_near(m1.values[1][0], 13.1f);
	assert_floats_near(m1.values[1][1], -1.1f);
	assert_floats_near(m1.values[1][2], 17.1f);
	assert_floats_near(m1.values[2][0], 90.2f);
	assert_floats_near(m1.values[2][1], 1.2f);
	assert_floats_near(m1.values[2][2], -2.2f);
}

TEST(matrix3x3f, matrix_matrix_multiplication)
{
	matrix3x3f const m1{
		2.0f, -3.0f, 11.0f,
		-1.0f, -1.0f, 0.15f,
		1.0f, 0.77f, 0.33f};
	matrix3x3f const m2{
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.25f,
		1.0f, 0.13f, 0.99f};
	matrix3x3f const m_muled{m1 * m2};
	assert_matrix3x3_near(
		m_muled,
		matrix3x3f{
			16.0f, 1.43f, 10.14f,
			0.15f, 0.0195f, -0.1015f,
			0.56f, 0.0429f, 0.5192f});
}

TEST(matrix3x3f, vector_matrix_multiplication)
{
	vector3f const v{1.0f, 3.0f, -0.15f};
	matrix3x3f const m{
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.25f,
		1.0f, 0.13f, 0.99f};
	vector3f const v_muled{v * m};
	assert_vectors3_near(v_muled, vector3f{-2.15f, -0.0195f, 0.6015f});
}

TEST(matrix3x3f, scaling)
{
	vector3f const v{2.0f, 3.0f, -0.15f};

	matrix3x3f const scale{matrix3x3f::scale(0.33f, 0.25f, 2.0f)};
	vector3f const v_scaled{v * scale};
	assert_vectors3_near(v_scaled, vector3f{0.66f, 0.75f, -0.30f});

	matrix3x3f const uniform_scale{matrix3x3f::uniform_scale(5.0f)};
	vector3f const v_uniformly_scaled{v * uniform_scale};
	assert_vectors3_near(v_uniformly_scaled, vector3f{10.0f, 15.0f, -0.75f});
}

TEST(matrix3x3f, rotation)
{
	vector3f const v{0.5f, 1.0f, -1.0f};

	matrix3x3f const m_rotation_around_x_axis{matrix3x3f::rotation_around_x_axis(static_cast<float>(M_PI) / 3.0f)};
	vector3f const v_rotated_around_x_axis{v * m_rotation_around_x_axis};
	assert_vectors3_near(v_rotated_around_x_axis, vector3f{0.5f, 1.366f, 0.366f});

	matrix3x3f const m_rotation_around_y_axis{matrix3x3f::rotation_around_y_axis(static_cast<float>(M_PI) / 10.0f)};
	vector3f const v_rotated_around_y_axis{v * m_rotation_around_y_axis};
	assert_vectors3_near(v_rotated_around_y_axis, vector3f{0.1665f, 1.0f, -1.1055f});

	matrix3x3f const m_rotation_around_z_axis{matrix3x3f::rotation_around_z_axis(static_cast<float>(M_PI) / 4.0f)};
	vector3f const v_rotated_around_z_axis{v * m_rotation_around_z_axis};
	assert_vectors3_near(v_rotated_around_z_axis, vector3f{-0.3535f, 1.0606f, -1.0f});

	vector3f const axis{0.55f, -1.0f, -1.0f};
	matrix3x3f const m_rotation_around_axis{matrix3x3f::rotation_around_axis(axis, static_cast<float>(M_PI) / 2.0f)};
	vector3f const v_rotated_around_axis{v * m_rotation_around_axis};
	assert_vectors3_near(v_rotated_around_axis, vector3f{1.3837f, -0.0864f, 0.5725f});
}
