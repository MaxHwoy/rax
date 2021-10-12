#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/tuple/tuple.hpp>

namespace rax::text
{
	class decoder final
	{
	private:
		RAX_INLINE static bool internal_utf8_to_utf16(const std::uint8_t* src, char16_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src < 0x80)
			{
				// max return value: 0x7F
				*dest = *src;

				src_off += 1u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xC0)
			{
				// minimum bit mask starts at 0xC0
				return false;
			}
			else if (*src < 0xE0)
			{
				// max result value: 0x7FF
				auto c1 = (src[0] & 0x1F) << 0x06;
				auto c2 = (src[1] & 0x3F);

				*dest = static_cast<char16_t>(c1 | c2);

				src_off += 2u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xF0)
			{
				// max result value: 0xFFFF
				auto c1 = (src[0] & 0x0F) << 0x0C;
				auto c2 = (src[1] & 0x3F) << 0x06;
				auto c3 = (src[2] & 0x3F);

				*dest = static_cast<char16_t>(c1 | c2 | c3);

				src_off += 3u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xF8)
			{
				// max result value: 0x10FFFF
				auto c1 = (src[0] & 0x07) << 0x12;
				auto c2 = (src[1] & 0x3F) << 0x0C;
				auto c3 = (src[2] & 0x3F) << 0x06;
				auto c4 = (src[3] & 0x3F);

				auto mask = (c1 | c2 | c3 | c4) - 0x10000;

				dest[0] = static_cast<char16_t>((mask >> 0x0A) + 0xD800);
				dest[1] = static_cast<char16_t>((mask & 0x3FF) + 0xDC00);

				src_off += 4u;
				dest_off += 2u;
				return true;
			}
			else
			{
				// max allowed bitmask is 0xF7
				return false;
			}
		}
		RAX_INLINE static bool internal_utf8_to_utf32(const std::uint8_t* src, char32_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src < 0x80)
			{
				// max return value: 0x7F
				*dest = *src;
				
				src_off += 1u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xC0)
			{
				// minimum bit mask starts at 0xC0
				return false;
			}
			else if (*src < 0xE0)
			{
				// max result value: 0x7FF
				auto c1 = (src[0] & 0x1F) << 0x06;
				auto c2 = (src[1] & 0x3F);

				*dest = c1 | c2;

				src_off += 2u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xF0)
			{
				// max result value: 0xFFFF
				auto c1 = (src[0] & 0x0F) << 0x0C;
				auto c2 = (src[1] & 0x3F) << 0x06;
				auto c3 = (src[2] & 0x3F);

				*dest = c1 | c2 | c3;

				src_off += 3u;
				dest_off += 1u;
				return true;
			}
			else if (*src < 0xF8)
			{
				// max result value: 0x10FFFF
				auto c1 = (src[0] & 0x07) << 0x12;
				auto c2 = (src[1] & 0x3F) << 0x0C;
				auto c3 = (src[2] & 0x3F) << 0x06;
				auto c4 = (src[3] & 0x3F);

				*dest = c1 | c2 | c3 | c4;

				src_off += 4u;
				dest_off += 1u;
				return true;
			}
			else
			{
				// max allowed bitmask is 0xF7
				return false;
			}
		}

		RAX_INLINE static void internal_utf16_to_utf8(const char16_t* src, std::uint8_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src < 0x80)
			{
				// 1 byte requred for bitmask 0x80
				dest[0] = static_cast<char>(*src);

				src_off += 1u;
				dest_off += 1u;
			}
			else if (*src < 0x800)
			{
				// 2 bytes required for bitmask 0x800
				dest[0] = static_cast<char>(0xC0 | (*src >> 0x06));
				dest[1] = static_cast<char>(0x80 | (*src & 0x03F));

				src_off += 1u;
				dest_off += 2u;
			}
			else if (*src >= 0xD800 && *src < 0xDC00)
			{
				// 4 bytes required for surrogates
				auto c1 = src[0] - 0xD800;
				auto c2 = src[1] - 0xDC00;

				auto fi = ((c1 << 0x0A) | c2) + 0x10000;

				dest[0] = static_cast<char>(0xF0 | (fi >> 0x12));
				dest[1] = static_cast<char>(0x80 | (fi >> 0x0C & 0x3F));
				dest[2] = static_cast<char>(0x80 | (fi >> 0x06 & 0x3F));
				dest[3] = static_cast<char>(0x80 | (fi & 0x03F));

				src_off += 2u;
				dest_off += 4u;
			}
			else
			{
				// 3 bytes required for bitmask 0x10000
				dest[0] = static_cast<char>(0xE0 | (*src >> 0x0C));
				dest[1] = static_cast<char>(0x80 | (*src >> 0x06 & 0x3F));
				dest[2] = static_cast<char>(0x80 | (*src & 0x03F));

				src_off += 1u;
				dest_off += 3u;
			}
		}
		RAX_INLINE static void internal_utf32_to_utf8(const char32_t* src, std::uint8_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src < 0x80)
			{
				// 1 byte requred for bitmask 0x80
				dest[0] = static_cast<char>(*src);

				src_off += 1u;
				dest_off += 1u;
			}
			else if (*src < 0x800)
			{
				// 2 bytes required for bitmask 0x800
				dest[0] = static_cast<char>(0xC0 | (*src >> 0x06));
				dest[1] = static_cast<char>(0x80 | (*src & 0x03F));
				
				src_off += 1u;
				dest_off += 2u;
			}
			else if (*src < 0x10000)
			{
				// 3 bytes required for bitmask 0x10000
				dest[0] = static_cast<char>(0xE0 | (*src >> 0x0C));
				dest[1] = static_cast<char>(0x80 | (*src >> 0x06 & 0x3F));
				dest[2] = static_cast<char>(0x80 | (*src & 0x03F));

				src_off += 1u;
				dest_off += 3u;
			}
			else
			{
				// 21 bits is the maximum required for any character
				dest[0] = static_cast<char>(0xF0 | (*src >> 0x12));
				dest[1] = static_cast<char>(0x80 | (*src >> 0x0C & 0x3F));
				dest[2] = static_cast<char>(0x80 | (*src >> 0x06 & 0x3F));
				dest[3] = static_cast<char>(0x80 | (*src & 0x03F));

				src_off += 1u;
				dest_off += 4u;
			}
		}

		RAX_INLINE static void internal_utf16_to_utf32(const char16_t* src, char32_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src >= 0xD800 && *src < 0xDC00)
			{
				// if surrogate utf16, we read 2 chars
				auto c1 = src[0] - 0xD800;
				auto c2 = src[1] - 0xDC00;

				*dest = ((c1 << 0x0A) | c2) + 0x10000;

				src_off += 2u;
				dest_off += 1u;
			}
			else
			{
				// everything else is 1 to 1
				*dest = *src;
				
				src_off += 1u;
				dest_off += 1u;
			}
		}
		RAX_INLINE static void internal_utf32_to_utf16(const char32_t* src, char16_t* dest, std::uint32_t& src_off, std::uint32_t& dest_off)
		{
			if (*src < 0x10000)
			{
				// if less than 0x10000, we fit in one char
				*dest = static_cast<char16_t>(*src);

				src_off += 1u;
				dest_off += 1u;
			}
			else
			{
				// otherwise split in two surrogates
				auto mask = *src - 0x10000;

				dest[0] = static_cast<char16_t>((mask >> 0x0A) + 0xD800);
				dest[1] = static_cast<char16_t>((mask & 0x3FF) + 0xDC00);

				src_off += 1u;
				dest_off += 2u;
			}
		}

	public:
		static auto utf8_to_utf16_char(const char* src, char16_t* dest) -> tuple2<std::int32_t, std::int32_t>;
		static auto utf16_to_utf8_char(const char16_t* src, char* dest) -> tuple2<std::int32_t, std::int32_t>;
		
		static auto utf8_to_utf32_char(const char* str, char32_t* dest) -> tuple2<std::int32_t, std::int32_t>;
		static auto utf32_to_utf8_char(const char32_t* src, char* dest) -> tuple2<std::int32_t, std::int32_t>;

		static auto utf16_to_utf32_char(const char16_t* src, char32_t* dest) -> tuple2<std::int32_t, std::int32_t>;
		static auto utf32_to_utf16_char(const char32_t* src, char16_t* dest) -> tuple2<std::int32_t, std::int32_t>;

		// we need utf7 too ?

		static auto utf8_to_utf16_string(const char* src, char16_t* dest) -> std::int32_t;
		static auto utf16_to_utf8_string(const char16_t* src, char* dest) -> std::int32_t;
		
		static auto utf8_to_utf32_string(const char* str, char32_t* dest) -> std::int32_t;
		static auto utf32_to_utf8_string(const char32_t* src, char* dest) -> std::int32_t;

		static auto utf16_to_utf32_string(const char16_t* src, char32_t* dest) -> std::int32_t;
		static auto utf32_to_utf16_string(const char32_t* src, char16_t* dest) -> std::int32_t;

		static auto utf8_to_utf16_string(const char* src, char16_t* dest, std::uint32_t length) -> std::int32_t;
		static auto utf16_to_utf8_string(const char16_t* src, char* dest, std::uint32_t length) -> std::int32_t;

		static auto utf8_to_utf32_string(const char* src, char32_t* dest, std::uint32_t length) -> std::int32_t;
		static auto utf32_to_utf8_string(const char32_t* src, char* dest, std::uint32_t length) -> std::int32_t;

		static auto utf16_to_utf32_string(const char16_t* src, char32_t* dest, std::uint32_t length) -> std::int32_t;
		static auto utf32_to_utf16_string(const char32_t* src, char16_t* dest, std::uint32_t length) -> std::int32_t;
	};
}
