#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/math.hpp>

namespace rax::numerics
{
	struct matrix4x4
	{
		float m11; // m00
		float m12; // m10
		float m13; // m20
		float m14; // m30

		float m21; // m01
		float m22; // m11
		float m23; // m21
		float m24; // m31

		float m31; // m02
		float m32; // m12
		float m33; // m22
		float m34; // m32

		float m41; // m03
		float m42; // m13
		float m43; // m23
		float m44; // m33
	};

	RAX_ASSERT_SIZE(matrix4x4, 0x40);
}
