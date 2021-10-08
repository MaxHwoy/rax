#include <gtest/gtest.h>

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	::testing::InitGoogleTest(&argc, (char**)argv);
	return RUN_ALL_TESTS();
}
