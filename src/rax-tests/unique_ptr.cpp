#include <gtest/gtest.h>
#include <rax/shared.hpp>
#include <rax/unique_ptr.hpp>

namespace rax::tests
{
	struct unique_ptr_test
	{
		RAX_INLINE unique_ptr_test(std::int32_t integer, const std::string& string, float floating)
			: integer(integer), string(string), floating(floating)
		{

		}

		std::int32_t integer;
		std::string string;
		float floating;
	};

	TEST(rax_unique_ptr, all)
	{
		auto first = rax::unique_ptr<std::int32_t>();
		auto second = rax::unique_ptr<std::int32_t>(3);
		auto third = rax::unique_ptr<unique_ptr_test>(1, "hello", 1337.420f);

		EXPECT_FALSE(first.get());

		EXPECT_EQ(*second, 3);

		second.~unique_ptr();

		EXPECT_TRUE(!second.get());

		EXPECT_EQ(third->integer, 1);
		EXPECT_TRUE(!third->string.compare("hello"));

		auto move = std::move(third);

		EXPECT_FALSE(third.get());
	}
}
