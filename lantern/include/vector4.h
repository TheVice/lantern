#ifndef LANTERN_VECTOR4_H
#define LANTERN_VECTOR4_H

namespace lantern
{
	/** Class representing 4-dimensional homogeneous vector.
	* Used to apply 3D affine transformations represented as 4x4 matrices
	* There is no templated version because there is no need for it
	*/
	class vector4f final
	{
	public:
		/** X-coordinate */
		float x;

		/** Y-coordinate */
		float y;

		/** Z-coordinate */
		float z;

		/** W-coordinate */
		float w;

		/** Constructs vector with zero coordinates */
		vector4f();

		/** Constructs vector with specified coordinates values */
		vector4f(float const x, float const y, float const z, float const w);
	};

	// vector4f implementation
	//

	inline vector4f::vector4f()
		: x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f}
	{
	}

	inline vector4f::vector4f(float const x, float const y, float const z, float const w)
		: x{x}, y{y}, z{z}, w{w}
	{

	}
}

#endif // LANTERN_VECTOR4_H
