#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/math.hpp>

namespace rax::collections
{
	class hash_helpers final
	{
	private:
		static inline const std::uint32_t kmax_prime_array_length_ = 2146435069u;
		static inline std::uint32_t primes_[] =
		{
			3u, 7u, 11u, 17u, 23u, 29u, 37u, 47u, 59u, 71u,
			89u, 107u, 131u, 163u, 197u, 239u, 293u, 353u, 431u, 521u,
			631u, 761u, 919u, 1103u, 1327u, 1597u, 1931u, 2333u, 2801u, 3371u,
			4049u, 4861u, 5839u, 7013u, 8419u, 10103u, 12143u, 14591u, 17519u, 21023u,
			25229u, 30293u, 36353u, 43627u, 52361u, 62851u, 75431u, 90523u, 108631u, 130363u,
			156437u, 187751u, 225307u, 270371u, 324449u, 389357u, 467237u, 560689u, 672827u, 807403u,
			968897u, 1162687u, 1395263u, 1674319u, 2009191u, 2411033u, 2893249u, 3471899u, 4166287u, 4999559u,
			5999471u, 7199369u
		};

	public:
		static auto get_prime(std::uint32_t number) -> std::uint32_t;
		static auto expand_prime(std::uint32_t number) -> std::uint32_t;

		RAX_INLINE static bool is_prime(std::uint32_t number)
		{
			return rax::math::is_prime(number);
		}
		RAX_INLINE static auto get_min_prime() -> std::uint32_t
		{
			return hash_helpers::primes_[0];
		}
	};
}
