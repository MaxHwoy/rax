#include <stdafx.hpp>
#include <vector>
#include <cuchar>

class cool_class
{
public:
	std::int32_t int_value;
	float float_value;
	rax::string string_value;
};

void test()
{
	auto reference = rax::refptr<cool_class>::make_ref();

	reference->string_value = "fucking hell";
	reference->float_value = 23.456f;

	*reference = cool_class();

	__debugbreak();
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

	return 0;
}
