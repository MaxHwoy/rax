#include <gtest/gtest.h>
#include <rax/text/decoder/decoder.hpp>

#pragma warning (disable : 4838) // conversion from 'int' to 'char' requires narrowing/cast
#pragma warning (disable : 4309) // 'initializing': truncation of constant value

namespace rax::tests
{
	TEST(rax_text_decoder, utf8_to_utf16_string)
	{
		const auto size1 = 44;
		const auto size2 = 24;

		char strx[size1] =
		{
			0x55, 0x54, 0x46, 0x38, 0x20, 0xD1, 0x8D, 0xD0,
			0xBD, 0xD0, 0xBA, 0xD0, 0xBE, 0xD0, 0xB4, 0xD0,
			0xB8, 0xD0, 0xBD, 0xD0, 0xB3, 0x20, 0xE2, 0x88,
			0x85, 0xE2, 0x88, 0xAE, 0xE2, 0x8A, 0xAD, 0xE2,
			0x8B, 0xAF, 0x20, 0xF0, 0x9F, 0xA9, 0xB8, 0xF0,
			0x9F, 0xAA, 0x90, 0x00
		};

		char16_t comp[size2] =
		{
			0x0055, 0x0054, 0x0046, 0x0038,
			0x0020, 0x044D, 0x043D, 0x043A,
			0x043E, 0x0434, 0x0438, 0x043D,
			0x0433, 0x0020, 0x2205, 0x222E,
			0x22AD, 0x22EF, 0x0020, 0xD83E,
			0xDE78, 0xD83E, 0xDE90, 0x0000
		};

		auto length1 = rax::text::decoder::utf8_to_utf16_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf8_to_utf16_string(strx + 5, nullptr, size1 - 5 - 9);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char16_t[length1];
		auto result2 = new char16_t[length2];

		auto actual1 = rax::text::decoder::utf8_to_utf16_string(strx, result1);
		auto actual2 = rax::text::decoder::utf8_to_utf16_string(strx + 5, result2, size1 - 5 - 9);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 10);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}

	TEST(rax_text_decoder, utf16_to_utf8_string)
	{
		const auto size1 = 24;
		const auto size2 = 44;

		char16_t strx[size1] =
		{
			0x0055, 0x0054, 0x0046, 0x0038,
			0x0020, 0x044D, 0x043D, 0x043A,
			0x043E, 0x0434, 0x0438, 0x043D,
			0x0433, 0x0020, 0x2205, 0x222E,
			0x22AD, 0x22EF, 0x0020, 0xD83E,
			0xDE78, 0xD83E, 0xDE90, 0x0000
		};

		char comp[size2] =
		{
			0x55, 0x54, 0x46, 0x38, 0x20, 0xD1, 0x8D, 0xD0,
			0xBD, 0xD0, 0xBA, 0xD0, 0xBE, 0xD0, 0xB4, 0xD0,
			0xB8, 0xD0, 0xBD, 0xD0, 0xB3, 0x20, 0xE2, 0x88,
			0x85, 0xE2, 0x88, 0xAE, 0xE2, 0x8A, 0xAD, 0xE2,
			0x8B, 0xAF, 0x20, 0xF0, 0x9F, 0xA9, 0xB8, 0xF0,
			0x9F, 0xAA, 0x90, 0x00
		};

		auto length1 = rax::text::decoder::utf16_to_utf8_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf16_to_utf8_string(strx + 5, nullptr, size1 - 5 - 5);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char[length1];
		auto result2 = new char[length2];

		auto actual1 = rax::text::decoder::utf16_to_utf8_string(strx, result1);
		auto actual2 = rax::text::decoder::utf16_to_utf8_string(strx + 5, result2, size1 - 5 - 5);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 14);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}

	TEST(rax_text_decoder, utf8_to_utf32_string)
	{
		const auto size1 = 44;
		const auto size2 = 22;

		char strx[size1] =
		{
			0x55, 0x54, 0x46, 0x38, 0x20, 0xD1, 0x8D, 0xD0,
			0xBD, 0xD0, 0xBA, 0xD0, 0xBE, 0xD0, 0xB4, 0xD0,
			0xB8, 0xD0, 0xBD, 0xD0, 0xB3, 0x20, 0xE2, 0x88,
			0x85, 0xE2, 0x88, 0xAE, 0xE2, 0x8A, 0xAD, 0xE2,
			0x8B, 0xAF, 0x20, 0xF0, 0x9F, 0xA9, 0xB8, 0xF0,
			0x9F, 0xAA, 0x90, 0x00
		};

		char32_t comp[size2] =
		{
			0x00000055, 0x00000054, 0x00000046, 0x00000038,
			0x00000020, 0x0000044D, 0x0000043D, 0x0000043A,
			0x0000043E, 0x00000434, 0x00000438, 0x0000043D,
			0x00000433, 0x00000020, 0x00002205, 0x0000222E,
			0x000022AD, 0x000022EF, 0x00000020, 0x0001FA78,
			0x0001FA90, 0x00000000
		};

		auto length1 = rax::text::decoder::utf8_to_utf32_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf8_to_utf32_string(strx + 5, nullptr, size1 - 5 - 9);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char32_t[length1];
		auto result2 = new char32_t[length2];

		auto actual1 = rax::text::decoder::utf8_to_utf32_string(strx, result1);
		auto actual2 = rax::text::decoder::utf8_to_utf32_string(strx + 5, result2, size1 - 5 - 9);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 8);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}

	TEST(rax_text_decoder, utf32_to_utf8_string)
	{
		const auto size1 = 22;
		const auto size2 = 44;

		char32_t strx[size1] =
		{
			0x00000055, 0x00000054, 0x00000046, 0x00000038,
			0x00000020, 0x0000044D, 0x0000043D, 0x0000043A,
			0x0000043E, 0x00000434, 0x00000438, 0x0000043D,
			0x00000433, 0x00000020, 0x00002205, 0x0000222E,
			0x000022AD, 0x000022EF, 0x00000020, 0x0001FA78,
			0x0001FA90, 0x00000000
		};

		char comp[size2] =
		{
			0x55, 0x54, 0x46, 0x38, 0x20, 0xD1, 0x8D, 0xD0,
			0xBD, 0xD0, 0xBA, 0xD0, 0xBE, 0xD0, 0xB4, 0xD0,
			0xB8, 0xD0, 0xBD, 0xD0, 0xB3, 0x20, 0xE2, 0x88,
			0x85, 0xE2, 0x88, 0xAE, 0xE2, 0x8A, 0xAD, 0xE2,
			0x8B, 0xAF, 0x20, 0xF0, 0x9F, 0xA9, 0xB8, 0xF0,
			0x9F, 0xAA, 0x90, 0x00
		};

		auto length1 = rax::text::decoder::utf32_to_utf8_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf32_to_utf8_string(strx + 5, nullptr, size1 - 5 - 3);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char[length1];
		auto result2 = new char[length2];

		auto actual1 = rax::text::decoder::utf32_to_utf8_string(strx, result1);
		auto actual2 = rax::text::decoder::utf32_to_utf8_string(strx + 5, result2, size1 - 5 - 3);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 14);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}

	TEST(rax_text_decoder, utf16_to_utf32_string)
	{
		const auto size1 = 24;
		const auto size2 = 22;

		char16_t strx[size1] =
		{
			0x0055, 0x0054, 0x0046, 0x0038,
			0x0020, 0x044D, 0x043D, 0x043A,
			0x043E, 0x0434, 0x0438, 0x043D,
			0x0433, 0x0020, 0x2205, 0x222E,
			0x22AD, 0x22EF, 0x0020, 0xD83E,
			0xDE78, 0xD83E, 0xDE90, 0x0000
		};

		char32_t comp[size2] =
		{
			0x00000055, 0x00000054, 0x00000046, 0x00000038,
			0x00000020, 0x0000044D, 0x0000043D, 0x0000043A,
			0x0000043E, 0x00000434, 0x00000438, 0x0000043D,
			0x00000433, 0x00000020, 0x00002205, 0x0000222E,
			0x000022AD, 0x000022EF, 0x00000020, 0x0001FA78,
			0x0001FA90, 0x00000000
		};

		auto length1 = rax::text::decoder::utf16_to_utf32_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf16_to_utf32_string(strx + 5, nullptr, size1 - 5 - 3);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char32_t[length1];
		auto result2 = new char32_t[length2];

		auto actual1 = rax::text::decoder::utf16_to_utf32_string(strx, result1);
		auto actual2 = rax::text::decoder::utf16_to_utf32_string(strx + 5, result2, size1 - 5 - 3);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 7);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}

	TEST(rax_text_decoder, utf32_to_utf16_string)
	{
		const auto size1 = 22;
		const auto size2 = 24;

		char32_t strx[size1] =
		{
			0x00000055, 0x00000054, 0x00000046, 0x00000038,
			0x00000020, 0x0000044D, 0x0000043D, 0x0000043A,
			0x0000043E, 0x00000434, 0x00000438, 0x0000043D,
			0x00000433, 0x00000020, 0x00002205, 0x0000222E,
			0x000022AD, 0x000022EF, 0x00000020, 0x0001FA78,
			0x0001FA90, 0x00000000
		};

		char16_t comp[size2] =
		{
			0x0055, 0x0054, 0x0046, 0x0038,
			0x0020, 0x044D, 0x043D, 0x043A,
			0x043E, 0x0434, 0x0438, 0x043D,
			0x0433, 0x0020, 0x2205, 0x222E,
			0x22AD, 0x22EF, 0x0020, 0xD83E,
			0xDE78, 0xD83E, 0xDE90, 0x0000
		};

		auto length1 = rax::text::decoder::utf32_to_utf16_string(strx, nullptr);
		auto length2 = rax::text::decoder::utf32_to_utf16_string(strx + 5, nullptr, size1 - 5 - 2);

		EXPECT_TRUE(length1 != -1 && length2 != -1 && length1 != length2);

		auto result1 = new char16_t[length1];
		auto result2 = new char16_t[length2];

		auto actual1 = rax::text::decoder::utf32_to_utf16_string(strx, result1);
		auto actual2 = rax::text::decoder::utf32_to_utf16_string(strx + 5, result2, size1 - 5 - 2);

		EXPECT_TRUE(length1 == actual1 && length2 == actual2);
		EXPECT_EQ(length1, size2);
		EXPECT_EQ(length2, size2 - 8);
		EXPECT_EQ(0, ::memcmp(comp, result1, length1));
		EXPECT_EQ(0, ::memcmp(comp + 5, result2, length2));

		delete[] result1;
		delete[] result2;
	}
}
