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
	rax::collections::templates::map<std::int32_t, std::int32_t> mapper;

	mapper[6] = 7;
	mapper[7] = 8;
	mapper[8] = 9;

	::printf("%d", mapper[7]);

	std::int32_t t = 0;
}

auto main(__int32 argc, const char** argv) -> std::int32_t
{
	test();

	return 0;
}
