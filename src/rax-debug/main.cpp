#include <stdafx.hpp>

#include <cuchar>

void test()
{
	auto nstr = "MaxHozy"; // 7
	auto wstr = L"IAmNotMax"; // 9

	auto nlen1 = rax::text::strex::strlen(nstr, false);
	auto nlen2 = rax::text::strex::strlen(nstr, true);

	auto wlen1 = rax::text::strex::strlen(wstr, false);
	auto wlen2 = rax::text::strex::strlen(wstr, true);

	auto name1 = rax::text::strex::strcmp(nstr, "Maxhozy");
	auto name2 = rax::text::strex::strcmp(nstr, "Max");

	auto wame1 = rax::text::strex::strcmp(wstr, L"IAmNotmax");
	auto wame2 = rax::text::strex::strcmp(wstr, L"IAmNot");

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
