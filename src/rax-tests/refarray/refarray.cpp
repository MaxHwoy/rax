#include <gtest/gtest.h>
#include <rax/refarray/refarray.hpp>

namespace rax::tests
{
	TEST(rax_refarray, binary_search)
	{
		auto arr = rax::refarray<std::int32_t>(51u);

		for (std::uint32_t i = 0u; i < 51u; ++i)
		{
			arr[i] = i << 3;
		}

		auto index1 = rax::refarray<std::int32_t>::binary_search(&arr, 35u << 3);
		auto index2 = rax::refarray<std::int32_t>::binary_search(&arr, 52u << 3);

		EXPECT_EQ(index1, 35);
		EXPECT_EQ(index2, -1);
	}

	TEST(rax_refarray, convert_all)
	{
		const std::uint32_t size = 10u;
	
		auto from = rax::refarray<std::int32_t>(size);

		for (std::uint32_t i = 0u; i < size; ++i)
		{
			from[i] = size << 2;
		}
	
		auto to = rax::refarray<std::int32_t>::convert_all<float>(&from, [](const std::int32_t* value) -> float
		{
			return (float)*value;
		});

		EXPECT_EQ(size, to.length());
	}
}
