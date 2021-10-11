#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax::text
{
	class decoder final
	{
	private:
		RAX_INLINE static auto internal_utf8_to_utf16(const char* str, char16_t& out) -> std::int32_t
		{
			if (*str < 0x80)
			{
				// max return value: 0x7F
				out = *str;
				return 1;
			}
			else if (*str < 0xC0)
			{
				// minimum bit mask starts at 0xC0
				return -1;
			}
			else if (*str < 0xE0)
			{
				// max result value: 0x7FF
				auto c1 = (str[0] & 0x1F) << 0x06;
				auto c2 = (str[1] & 0x3F);

				out = static_cast<char16_t>(c1 | c2);
				return 2;
			}
			else if (*str < 0xF0)
			{
				// max result value: 0xFFFF
				auto c1 = (str[0] & 0x0F) << 0x0C;
				auto c2 = (str[1] & 0x3F) << 0x06;
				auto c3 = (str[2] & 0x3F);

				out = static_cast<char16_t>(c1 | c2 | c3);
				return 3;
			}
			else
			{
				// any other utf8 characters that use
				// 4 bytes do not fit in regular utf16
				// use utf32 instead for full conversion
				return -1;
			}
		}
		RAX_INLINE static auto internal_utf8_to_utf32(const char* str, char32_t& out) -> std::int32_t
		{
			if (*str < 0x80)
			{
				// max return value: 0x7F
				out = *str;
				return 1;
			}
			else if (*str < 0xC0)
			{
				// minimum bit mask starts at 0xC0
				return -1;
			}
			else if (*str < 0xE0)
			{
				// max result value: 0x7FF
				auto c1 = (str[0] & 0x1F) << 0x06;
				auto c2 = (str[1] & 0x3F);

				out = c1 | c2;
				return 2;
			}
			else if (*str < 0xF0)
			{
				// max result value: 0xFFFF
				auto c1 = (str[0] & 0x0F) << 0x0C;
				auto c2 = (str[1] & 0x3F) << 0x06;
				auto c3 = (str[2] & 0x3F);

				out = c1 | c2 | c3;
				return 3;
			}
			else if (*str < 0xF8)
			{
				// max result value: 0x10FFFF
				auto c1 = (str[0] & 0x07) << 0x12;
				auto c2 = (str[1] & 0x3F) << 0x0C;
				auto c3 = (str[2] & 0x3F) << 0x06;
				auto c4 = (str[3] & 0x3F);

				// this actually should be illegalized
				// and everything should use utf16, thx
				out = c1 | c2 | c3 | c4;
				return 4;
			}
			else
			{
				// max allowed bitmask is 0xF7
				return -1;
			}
		}

		RAX_INLINE static auto internal_utf16_to_utf8(char16_t val, char* dest) -> std::int32_t
		{
			if (val < 0x80)
			{
				// 1 byte requred for bitmask 0x80
				dest[0] = static_cast<char>(val);
				return 1;
			}
			else if (val < 0x800)
			{
				// 2 bytes required for bitmask 0x800
				dest[0] = static_cast<char>(0xC0 | (val >> 0x06));
				dest[1] = static_cast<char>(0x80 | (val & 0x03F));
				return 2;
			}
			else
			{
				// 3 bytes required for bitmask 0x10000
				dest[0] = static_cast<char>(0xE0 | (val >> 0x0C));
				dest[1] = static_cast<char>(0x80 | (val >> 0x06 & 0x3F));
				dest[2] = static_cast<char>(0x80 | (val & 0x03F));

				return 3;
			}
		}
		RAX_INLINE static auto internal_utf32_to_utf8(char32_t val, char* dest) -> std::int32_t
		{
			if (val < 0x80)
			{
				// 1 byte requred for bitmask 0x80
				dest[0] = static_cast<char>(val);
				return 1;
			}
			else if (val < 0x800)
			{
				// 2 bytes required for bitmask 0x800
				dest[0] = static_cast<char>(0xC0 | (val >> 0x06));
				dest[1] = static_cast<char>(0x80 | (val & 0x03F));
				return 2;
			}
			else if (val < 0x10000)
			{
				// 3 bytes required for bitmask 0x10000
				dest[0] = static_cast<char>(0xE0 | (val >> 0x0C));
				dest[1] = static_cast<char>(0x80 | (val >> 0x06 & 0x3F));
				dest[2] = static_cast<char>(0x80 | (val & 0x03F));

				return 3;
			}
			else
			{
				// 21 bits is the maximum required for any character
				dest[0] = static_cast<char>(0xF0 | (val >> 0x12));
				dest[1] = static_cast<char>(0x80 | (val >> 0x0C & 0x3F));
				dest[2] = static_cast<char>(0x80 | (val >> 0x06 & 0x3F));
				dest[3] = static_cast<char>(0x80 | (val & 0x03F));

				return 4;
			}
		}

	public:
		static auto utf8_to_utf16(const char* str, char16_t& out) -> std::int32_t;
		static auto utf16_to_utf8(char16_t str, char* dest) -> std::int32_t;
		
		static auto utf8_to_utf32(const char* str, char32_t& out) -> std::int32_t;
		static auto utf32_to_utf8(char32_t val, char* dest) -> std::int32_t;

		// we need utf7 too ?

		static auto utf8_to_utf16(const char* src, char16_t* dest) -> std::int32_t;
		static auto utf16_to_utf8(const char16_t* src, char* dest) -> std::int32_t;
		
		static auto utf8_to_utf32(const char* str, char32_t* dest) -> std::int32_t;
		static auto utf32_to_utf8(const char32_t* src, char* dest) -> std::int32_t;

		static auto utf8_to_utf16(const char* src, char16_t* dest, std::uint32_t length) -> std::int32_t;
		static auto utf16_to_utf8(const char16_t* src, char* dest, std::uint32_t length) -> std::int32_t;

		static auto utf8_to_utf32(const char* src, char32_t* dest, std::uint32_t length) -> std::int32_t;
		static auto utf32_to_utf8(const char32_t* src, char* dest, std::uint32_t length) -> std::int32_t;
	};
}
