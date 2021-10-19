#include <gtest/gtest.h>
#include <rax/text/string_builder.hpp>

namespace rax::tests
{
	TEST(rax_string_builder, all)
	{
		auto sb1 = text::string_builder();

		EXPECT_FALSE(sb1.string().length());

		sb1.append("stop posting about among us! ");
		sb1.append("i'm tired of seeing it! ");
		sb1.append("my friends on tiktok send me memes, on discord its fucking memes!");

		EXPECT_TRUE(sb1.string().length());

		// to be done when rax::string::replace!
		// sb1.replace("among us", "discord");

		sb1.substring(0, 12);

		auto stop_posting = sb1.string();

		EXPECT_TRUE(rax::string::equals(stop_posting, "stop posting"));
	}
}
