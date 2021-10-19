#include <stdafx.hpp>
#include <vector>
#include <cuchar>

void test()
{
	__debugbreak();
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

	return 0;
}
