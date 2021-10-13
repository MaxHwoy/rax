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

	TEST(rax_string, operator_plus)
	{
		string a = "Stop talking ";
		string b = "about Discord";
		string c = a + b;

		EXPECT_EQ(c, "Stop talking about Discord");

		string d = "I am tired";
		const char* e = " of Discord!";
		string f = d + e;
		string g = e + d;

		EXPECT_EQ(f, g);
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
