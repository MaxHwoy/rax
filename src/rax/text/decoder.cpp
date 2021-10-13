#include <rax/text/decoder.hpp>

namespace rax::text
{
	auto decoder::utf8_to_utf16_char(const char* src, char16_t* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;
		
		if (decoder::internal_utf8_to_utf16(reinterpret_cast<const std::uint8_t*>(src), dest, src_res, dst_res))
		{
			return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
		}
		else
		{
			return tuple2<std::int32_t, std::int32_t>(-1, -1);
		}
	}
	
	auto decoder::utf16_to_utf8_char(const char16_t* src, char* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;

		decoder::internal_utf16_to_utf8(src, reinterpret_cast<std::uint8_t*>(dest), src_res, dst_res);

		return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
	}

	auto decoder::utf8_to_utf32_char(const char* src, char32_t* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;

		if (decoder::internal_utf8_to_utf32(reinterpret_cast<const std::uint8_t*>(src), dest, src_res, dst_res))
		{
			return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
		}
		else
		{
			return tuple2<std::int32_t, std::int32_t>(-1, -1);
		}
	}

	auto decoder::utf32_to_utf8_char(const char32_t* src, char* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;

		decoder::internal_utf32_to_utf8(src, reinterpret_cast<std::uint8_t*>(dest), src_res, dst_res);

		return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
	}

	auto decoder::utf16_to_utf32_char(const char16_t* src, char32_t* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;

		decoder::internal_utf16_to_utf32(src, dest, src_res, dst_res);

		return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
	}

	auto decoder::utf32_to_utf16_char(const char32_t* src, char16_t* dest) -> tuple2<std::int32_t, std::int32_t>
	{
		std::uint32_t src_res = 0u;
		std::uint32_t dst_res = 0u;

		decoder::internal_utf32_to_utf16(src, dest, src_res, dst_res);

		return tuple2<std::int32_t, std::int32_t>(static_cast<std::int32_t>(src_res), static_cast<std::int32_t>(dst_res));
	}



	auto decoder::utf8_to_utf16_string(const char* src, char16_t* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		auto ptr = reinterpret_cast<const std::uint8_t*>(src);

		if (dest == nullptr)
		{
			char16_t buffer[2];

			while (src[src_off] != NULL)
			{
				if (!decoder::internal_utf8_to_utf16(ptr + src_off, buffer, src_off, dst_off))
				{
					return -1;
				}
			}
		}
		else
		{
			while (src[src_off] != NULL)
			{
				if (!decoder::internal_utf8_to_utf16(ptr + src_off, dest + dst_off, src_off, dst_off))
				{
					return -1;
				}
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf16_to_utf8_string(const char16_t* src, char* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			std::uint8_t buffer[4];

			while (src[src_off] != NULL)
			{
				decoder::internal_utf16_to_utf8(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			auto ptr = reinterpret_cast<std::uint8_t*>(dest);

			while (src[src_off] != NULL)
			{
				decoder::internal_utf16_to_utf8(src + src_off, ptr + dst_off, src_off, dst_off);
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf8_to_utf32_string(const char* src, char32_t* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		auto ptr = reinterpret_cast<const std::uint8_t*>(src);

		if (dest == nullptr)
		{
			char32_t buffer[1];

			while (src[src_off] != NULL)
			{
				if (!decoder::internal_utf8_to_utf32(ptr + src_off, buffer, src_off, dst_off))
				{
					return -1;
				}
			}
		}
		else
		{
			while (src[src_off] != NULL)
			{
				if (!decoder::internal_utf8_to_utf32(ptr + src_off, dest + dst_off, src_off, dst_off))
				{
					return -1;
				}
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf32_to_utf8_string(const char32_t* src, char* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			std::uint8_t buffer[4];

			while (src[src_off] != NULL)
			{
				decoder::internal_utf32_to_utf8(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			auto ptr = reinterpret_cast<std::uint8_t*>(dest);

			while (src[src_off] != NULL)
			{
				decoder::internal_utf32_to_utf8(src + src_off, ptr + dst_off, src_off, dst_off);
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf16_to_utf32_string(const char16_t* src, char32_t* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			char32_t buffer[1];

			while (src[src_off] != NULL)
			{
				decoder::internal_utf16_to_utf32(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			while (src[src_off] != NULL)
			{
				decoder::internal_utf16_to_utf32(src + src_off, dest + dst_off, src_off, dst_off);
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf32_to_utf16_string(const char32_t* src, char16_t* dest) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			char16_t buffer[2];

			while (src[src_off] != NULL)
			{
				decoder::internal_utf32_to_utf16(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			while (src[src_off] != NULL)
			{
				decoder::internal_utf32_to_utf16(src + src_off, dest + dst_off, src_off, dst_off);
			}

			dest[dst_off] = 0;
		}

		return static_cast<std::int32_t>(dst_off + 1u);
	}

	auto decoder::utf8_to_utf16_string(const char* src, char16_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		auto ptr = reinterpret_cast<const std::uint8_t*>(src);

		if (dest == nullptr)
		{
			char16_t buffer[2];

			while (src_off < length)
			{
				if (!decoder::internal_utf8_to_utf16(ptr + src_off, buffer, src_off, dst_off))
				{
					return -1;
				}
			}
		}
		else
		{
			while (src_off < length)
			{
				if (!decoder::internal_utf8_to_utf16(ptr + src_off, dest + dst_off, src_off, dst_off))
				{
					return -1;
				}
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}

	auto decoder::utf16_to_utf8_string(const char16_t* src, char* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			std::uint8_t buffer[4];

			while (src_off < length)
			{
				decoder::internal_utf16_to_utf8(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			auto ptr = reinterpret_cast<std::uint8_t*>(dest);

			while (src_off < length)
			{
				decoder::internal_utf16_to_utf8(src + src_off, ptr + dst_off, src_off, dst_off);
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}

	auto decoder::utf8_to_utf32_string(const char* src, char32_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		auto ptr = reinterpret_cast<const std::uint8_t*>(src);

		if (dest == nullptr)
		{
			char32_t buffer[1];

			while (src_off < length)
			{
				if (!decoder::internal_utf8_to_utf32(ptr + src_off, buffer, src_off, dst_off))
				{
					return -1;
				}
			}
		}
		else
		{
			while (src_off < length)
			{
				if (!decoder::internal_utf8_to_utf32(ptr + src_off, dest + dst_off, src_off, dst_off))
				{
					return -1;
				}
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}

	auto decoder::utf32_to_utf8_string(const char32_t* src, char* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			std::uint8_t buffer[4];

			while (src_off < length)
			{
				decoder::internal_utf32_to_utf8(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			auto ptr = reinterpret_cast<std::uint8_t*>(dest);

			while (src_off < length)
			{
				decoder::internal_utf32_to_utf8(src + src_off, ptr + dst_off, src_off, dst_off);
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}

	auto decoder::utf16_to_utf32_string(const char16_t* src, char32_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			char32_t buffer[1];

			while (src_off < length)
			{
				decoder::internal_utf16_to_utf32(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			while (src_off < length)
			{
				decoder::internal_utf16_to_utf32(src + src_off, dest + dst_off, src_off, dst_off);
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}

	auto decoder::utf32_to_utf16_string(const char32_t* src, char16_t* dest, std::uint32_t length) -> std::int32_t
	{
		std::uint32_t src_off = 0u;
		std::uint32_t dst_off = 0u;

		if (dest == nullptr)
		{
			char16_t buffer[2];

			while (src_off < length)
			{
				decoder::internal_utf32_to_utf16(src + src_off, buffer, src_off, dst_off);
			}
		}
		else
		{
			while (src_off < length)
			{
				decoder::internal_utf32_to_utf16(src + src_off, dest + dst_off, src_off, dst_off);
			}
		}

		return static_cast<std::int32_t>(dst_off);
	}
}
