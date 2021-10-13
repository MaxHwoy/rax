#include <stdafx.hpp>

#include <cuchar>

void test()
{
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
