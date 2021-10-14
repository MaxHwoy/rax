#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax
{
	class bitwise final
	{
	public:
		RAX_INLINE static auto align_pow_2(std::uint32_t value, std::uint32_t pow2)
		{
			auto diff = pow2 - (value & (pow2 - 1u));
			return value + (diff == pow2 ? 0 : diff);
		}

		RAX_INLINE static auto byte_swap(std::uint32_t value) -> std::uint32_t
		{
			return (value << 24) | ((value << 16) >> 8) | ((value << 8) >> 16) | (value >> 24);
		}

		RAX_INLINE static bool is_pow_2(std::uint32_t value)
		{
			return value > 0 && (value & (value - 1)) == 0;
		}

		RAX_INLINE static auto next_pow_2(std::uint32_t value) -> std::uint32_t
		{
			//if (value == 0)
			//{
			//	return 0; // if only we could get rid of this if
			//}

			--value;

			value |= value >> 1;
			value |= value >> 2;
			value |= value >> 4;
			value |= value >> 8;
			value |= value >> 16;

			return ++value;
		}
		RAX_INLINE static auto prev_pow_2(std::uint32_t value) -> std::uint32_t
		{
			value |= value >> 1;
			value |= value >> 2;
			value |= value >> 4;
			value |= value >> 8;
			value |= value >> 16;

			return value - (value >> 1);
		}

		RAX_INLINE static auto rotate_left(std::uint32_t value, std::uint32_t offset) -> std::uint32_t
		{
			return (value << offset) | (value >> (32 - offset));
		}
		RAX_INLINE static auto rotate_right(std::uint32_t value, std::uint32_t offset) -> std::uint32_t
		{
			return (value >> offset) | (value << (32 - offset));
		}
	};
}
