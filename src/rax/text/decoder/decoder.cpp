#include <rax/text/decoder/decoder.hpp>

namespace rax::text
{
	auto decoder::utf8_to_utf16(const char* str, char16_t& out) -> std::int32_t
	{
		return decoder::internal_utf8_to_utf16(str, out);
	}
	
	auto decoder::utf16_to_utf8(char16_t val, char* dest) -> std::int32_t
	{
		return decoder::internal_utf16_to_utf8(val, dest);
	}

	auto decoder::utf8_to_utf32(const char* str, char32_t& out) -> std::int32_t
	{
		return decoder::internal_utf8_to_utf32(str, out);
	}

	auto decoder::utf32_to_utf8(char32_t val, char* dest) -> std::int32_t
	{
		return decoder::internal_utf32_to_utf8(val, dest);
	}




	auto decoder::utf8_to_utf16(const char* src, char16_t* dest) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;

		char16_t c = 0;

		if (dest == nullptr)
		{
			while (*src != NULL)
			{
				auto convert = decoder::internal_utf8_to_utf16(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					srcoff += convert;
					++dstoff;
				}
			}

			++dstoff; // include null-terminator
		}
		else
		{
			while (*src != NULL)
			{
				auto convert = decoder::internal_utf8_to_utf16(src + srcoff, c);

				if (convert == -1)
				{
					dest[dstoff] = 0; // null terminate for safety
					return -1;
				}
				else
				{
					dest[dstoff] = c;

					srcoff += convert;
					++dstoff;
				}
			}

			dest[dstoff++] = 0; // add null-terminator
		}

		return dstoff;
	}

	auto decoder::utf16_to_utf8(const char16_t* src, char* dest) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;

		if (dest == nullptr)
		{
			char buffer[4];

			while (*src != NULL)
			{
				auto convert = decoder::utf16_to_utf8(src[srcoff], buffer);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}

			++dstoff; // include null-terminator
		}
		else
		{
			while (*src != NULL)
			{
				auto convert = decoder::utf16_to_utf8(src[srcoff], dest + dstoff);

				if (convert == -1)
				{
					dest[dstoff] = 0; // null terminate for safety
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}

			dest[dstoff++] = 0; // add null-terminator
		}

		return dstoff;
	}

	auto decoder::utf8_to_utf32(const char* src, char32_t* dest) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;

		char32_t c = 0;

		if (dest == nullptr)
		{
			while (*src != NULL)
			{
				auto convert = decoder::internal_utf8_to_utf32(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					srcoff += convert;
					++dstoff;
				}
			}

			++dstoff; // include null-terminator
		}
		else
		{
			while (*src != NULL)
			{
				auto convert = decoder::internal_utf8_to_utf32(src + srcoff, c);

				if (convert == -1)
				{
					dest[dstoff] = 0; // null terminate for safety
					return -1;
				}
				else
				{
					dest[dstoff] = c;

					srcoff += convert;
					++dstoff;
				}
			}

			dest[dstoff++] = 0; // add null-terminator
		}

		return dstoff;
	}

	auto decoder::utf32_to_utf8(const char32_t* src, char* dest) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;

		if (dest == nullptr)
		{
			char buffer[4];

			while (*src != NULL)
			{
				auto convert = decoder::utf32_to_utf8(src[srcoff], buffer);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}

			++dstoff; // include null-terminator
		}
		else
		{
			while (*src != NULL)
			{
				auto convert = decoder::utf32_to_utf8(src[srcoff], dest + dstoff);

				if (convert == -1)
				{
					dest[dstoff] = 0; // null terminate for safety
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}

			dest[dstoff++] = 0; // add null-terminator
		}

		return dstoff;
	}

	auto decoder::utf8_to_utf16(const char* src, char16_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;
		std::int32_t counts = static_cast<std::int32_t>(length);

		char16_t c = 0;

		if (dest == nullptr)
		{
			while (srcoff < counts)
			{
				auto convert = decoder::internal_utf8_to_utf16(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					srcoff += convert;
					++dstoff;
				}
			}
		}
		else
		{
			while (srcoff < counts)
			{
				auto convert = decoder::internal_utf8_to_utf16(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dest[dstoff] = c;

					srcoff += convert;
					++dstoff;
				}
			}
		}

		return dstoff;
	}

	auto decoder::utf16_to_utf8(const char16_t* src, char* dest, std::uint32_t length) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;
		std::int32_t counts = static_cast<std::int32_t>(length);

		if (dest == nullptr)
		{
			char buffer[4];

			while (srcoff < counts)
			{
				auto convert = decoder::utf16_to_utf8(src[srcoff], buffer);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}
		}
		else
		{
			while (srcoff < counts)
			{
				auto convert = decoder::utf16_to_utf8(src[srcoff], dest + dstoff);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}
		}

		return dstoff;
	}

	auto decoder::utf8_to_utf32(const char* src, char32_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;
		std::int32_t counts = static_cast<std::int32_t>(length);

		char32_t c = 0;

		if (dest == nullptr)
		{
			while (srcoff < counts)
			{
				auto convert = decoder::internal_utf8_to_utf32(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					srcoff += convert;
					++dstoff;
				}
			}
		}
		else
		{
			while (srcoff < counts)
			{
				auto convert = decoder::internal_utf8_to_utf32(src + srcoff, c);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dest[dstoff] = c;

					srcoff += convert;
					++dstoff;
				}
			}
		}

		return dstoff;
	}

	auto decoder::utf32_to_utf8(const char32_t* src, char* dest, std::uint32_t length) -> std::int32_t
	{
		std::int32_t srcoff = 0;
		std::int32_t dstoff = 0;
		std::int32_t counts = static_cast<std::int32_t>(length);

		if (dest == nullptr)
		{
			char buffer[4];

			while (srcoff < counts)
			{
				auto convert = decoder::utf32_to_utf8(src[srcoff], buffer);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}
		}
		else
		{
			while (srcoff < counts)
			{
				auto convert = decoder::utf32_to_utf8(src[srcoff], dest + dstoff);

				if (convert == -1)
				{
					return -1;
				}
				else
				{
					dstoff += convert;
					++srcoff;
				}
			}
		}

		return dstoff;
	}
}
