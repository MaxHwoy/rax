#pragma once

#include <cstdint>
#include <tgmath.h>
#include <rax/shared.hpp>
#include <rax/math.hpp>

namespace rax::numerics
{
	struct vector2
	{
		static const vector2 zero;
		static const vector2 one;

		float x;
		float y;

		RAX_INLINE vector2() : x(0.0f), y(0.0f)
		{
		}

		RAX_INLINE vector2(float x, float y) : x(x), y(y)
		{
		}

		RAX_INLINE vector2(const vector2& other) : x(other.x), y(other.y)
		{
		}

		RAX_INLINE vector2& operator=(const vector2& other)
		{
			this->x = other.x;
			this->y = other.y;
		}

		RAX_INLINE auto operator[](std::uint32_t index) -> float&
		{
			return reinterpret_cast<float*>(this)[index];
		}
		RAX_INLINE auto operator[](std::int32_t index) -> float&
		{
			return reinterpret_cast<float*>(this)[index];
		}


		RAX_INLINE static auto distance(vector2 a, vector2 b) -> float
		{
			float dx = b.x - a.x;
			float dy = b.y - a.y;

			return ::sqrtf(dx * dx + dy * dy);
		}

		RAX_INLINE static auto dot(vector2 a, vector2 b) -> float
		{
			return a.x * b.x + a.y * b.y;
		}


		void clamp_magnitude(float max_length);




		friend bool operator==(vector2 lhs, vector2 rhs);
		friend bool operator!=(vector2 lhs, vector2 rhs);
	};

	RAX_ASSERT_SIZE(vector2, 0x08);
}
