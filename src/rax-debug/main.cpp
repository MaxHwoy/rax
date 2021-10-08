#include <stdafx.hpp>
#include <cstdint>
#include <stdio.h>
#include <string>

#include <rax/stdafx.hpp>
#include <rax/refarray/refarray.hpp>
#include <rax/tuple/tuple.hpp>
#include <rax/math/math.hpp>
//#include <rax/numerics/vector/vector.hpp>

void test()
{
	auto minmax = rax::math::minmax(5, 7);

	auto arr = rax::refarray<int>(6u);

	auto fuc = arr;

	for (std::uint32_t i = 0u; i < 6u; ++i)
	{
		arr[i] = i << 2;
	}

	rax::refarray<int>::resize(arr, 8u);

	auto sarr = rax::refarray<float>::empty();
	
	sarr = rax::refarray<float>(40u);
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

#ifndef DEBUG_ARRAY_PRINT
	::printf("Allocated array count: %d\n", rax::array_debug_info::num_allocated_arrays);
	::printf("Destroyed array count: %d\n", rax::array_debug_info::num_destroyed_arrays);
#endif // !DEBUG_ARRAY_PRINT

	return 0;
}
