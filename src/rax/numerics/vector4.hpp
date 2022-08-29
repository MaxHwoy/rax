#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/math.hpp>

namespace rax::numerics
{
	struct vector4
	{
		float x;
		float y;
		float z;
		float w;

		vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}
	};

	RAX_ASSERT_SIZE(vector4, 0x10);
}
