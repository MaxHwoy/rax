#include <gtest/gtest.h>
#include <rax/shared.hpp>
#include <rax/optional.hpp>

namespace rax::tests
{
	TEST(rax, optional)
	{
		auto first = rax::optional<int>();

		EXPECT_FALSE(first.has_value());
		EXPECT_FALSE(first.operator bool());

		EXPECT_EQ(first.value(), 0);

		constexpr auto the_value = 1337.0f;

		const auto second = rax::optional<float>(the_value);

		EXPECT_TRUE(second.has_value());
		EXPECT_TRUE(second.operator bool());

		EXPECT_EQ(second.value(), the_value);
	}
}
