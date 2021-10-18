#include <gtest/gtest.h>
#include <rax/shared.hpp>
#include <rax/optional.hpp>

namespace rax::tests
{
	TEST(rax_optional, all)
	{
		auto first = rax::optional<std::int32_t>();

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
