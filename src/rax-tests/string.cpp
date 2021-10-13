#include <gtest/gtest.h>
#include <rax/string.hpp>

namespace rax::tests
{
	TEST(rax_string, ctor)
	{
		auto ptr = "custom string";

		string str1 = string(ptr);
		string str2 = ptr;

		EXPECT_EQ(str1, str2);
	}

	TEST(rax_string, substring)
	{
		string str = "rax::string::substring";
		string substr = str.substring(5u);
		string strsub = str.substring(5u, 6u);

		EXPECT_EQ(substr, "string::substring");
		EXPECT_EQ(strsub, "string");
	}
}
