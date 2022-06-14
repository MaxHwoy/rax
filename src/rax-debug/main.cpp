#include <stdafx.hpp>
#include <vector>
#include <cuchar>

#include <atomic>
#include <thread>
#include <type_traits>
#include <unordered_map>

#include <rax/objective.hpp>


struct alloc
{
	void run() { ::printf("7\n"); }
};

struct reall
{
	void run() { ::printf("8\n"); }
};

template <typename T = alloc> struct something
{
	void perform()
	{
		T().run();
	}
};


void test()
{
	auto ref = __makeref(std::int32_t);
	auto arr = rax::refarray<std::int32_t>(2u);

	*ref = 5;

	::printf("%d\n", *ref);

	auto some = something<alloc>();
	auto oofe = something<reall>();

	some.perform();
	oofe.perform();

	auto key = rax::hash<std::int32_t>()(1000);
	
	std::int32_t t = 0;
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

	return 0;
}
