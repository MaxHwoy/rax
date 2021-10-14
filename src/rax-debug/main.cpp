#include <stdafx.hpp>

#include <cuchar>

void test()
{
	auto mymap = rax::collections::templates::map<std::uint32_t, std::int32_t>
	{
		{ 5u, 3 },
		{ 9u, 4 },
		{ 4u, 1 },
		{ 12u, 9 },
	};

	mymap.put(7u, 13);

	std::int32_t result = 0;

	if (mymap.try_get_value(7u, result))
	{
		assert(result == 13);
	}

	__debugbreak();
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

#ifndef DEBUG_ARRAY_PRINT
	::printf("Allocated array count: %d\n", rax::refarray_debug_info::num_allocated_refarrays);
	::printf("Destroyed array count: %d\n", rax::refarray_debug_info::num_destroyed_refarrays);
#endif // !DEBUG_ARRAY_PRINT

	return 0;
}
