#include <stdafx.hpp>
#include <cstdint>

#include <rax/stdafx.hpp>
#include <rax/tuple/tuple.hpp>
#include <rax/math/math.hpp>
#include <rax/numerics/vector/vector.hpp>

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	auto stuff = rax::math::minmax(5.0, 7.0);

	return 0;
}
