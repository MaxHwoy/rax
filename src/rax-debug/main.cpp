#include <stdafx.hpp>
#include <cstdint>
#include <stdio.h>

#include <rax/stdafx.hpp>
#include <rax/array/array.hpp>
#include <rax/tuple/tuple.hpp>
#include <rax/math/math.hpp>
#include <rax/numerics/vector/vector.hpp>

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	auto arr = rax::array<int>(6u);

	auto fuc = arr;

	for (std::uint32_t i = 0u; i < 6u; ++i)
	{
		arr[i] = i << 2;
	}

	rax::array<int>::resize(arr, 8u);

	for (std::uint32_t i = 0u; i < arr.length(); ++i)
	{
		::printf("%d ", arr[i]);
	}

	::printf("\n");

	return 0;
}
