#include <gtest/gtest.h>
#include <rax/shared.hpp>
#include <rax/span.hpp>

namespace rax::tests
{
	TEST(rax_span, all)
	{
		std::int32_t array_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::uint8_t array_2[100]{};

		auto i = 0ui8;

		for (auto& byte : array_2)
		{
			byte = i++;
		}

		auto span_1 = rax::span(array_1);

		EXPECT_EQ(span_1.begin(), &array_1[0]);

		auto span_2 = rax::span(array_2);
		auto subspan_of_2 = span_2.subspan(50);

		EXPECT_EQ(subspan_of_2.begin(), &array_2[50]);
		EXPECT_EQ(subspan_of_2.size() * 2, std::size(array_2));
	}
}
