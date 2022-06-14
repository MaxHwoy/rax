#include <rax/math.hpp>



namespace rax
{
	auto math::sqrt(float value) -> float
	{
		const auto result = ::_mm_sqrt_ss(::_mm_load_ss(&value));

		return result.m128_f32[0];
	}
}
