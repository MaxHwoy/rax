#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/math.hpp>
#include <rax/numerics/vector2.hpp>

namespace rax::numerics
{
	struct vector3
	{
		float x;
		float y;
		float z;

		RAX_INLINE vector3() : x(0.0f), y(0.0f), z(0.0f)
		{
		}

		RAX_INLINE vector3(vector2 v) : x(v.x), y(v.y), z(0.0f)
		{
		}

		RAX_INLINE vector3(const vector3& v) : x(v.x), y(v.y), z(v.z)
		{
		}




	};

	RAX_ASSERT_SIZE(vector3, 0x0C);
}
