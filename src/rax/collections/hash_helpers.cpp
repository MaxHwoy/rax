#include <rax/collections/hash_helpers.hpp>

namespace rax::collections
{
	auto hash_helpers::get_prime(std::uint32_t number) -> std::uint32_t
	{
		auto length = sizeof(hash_helpers::primes_) / sizeof(std::uint32_t);

		for (std::uint32_t i = 0; i < length; ++i)
		{
			auto num = hash_helpers::primes_[i];

			if (num >= number)
			{
				return num;
			}
		}

		for (auto j = number | 1u; j < UINT32_MAX; j += 2)
		{
			if (hash_helpers::is_prime(j) && (j - 1) % 101 != 0)
			{
				return j;
			}
		}

		return number;
	}

	auto hash_helpers::expand_prime(std::uint32_t number) -> std::uint32_t
	{
		auto num = number << 1;

		if (num > hash_helpers::kmax_prime_array_length_ &&
			hash_helpers::kmax_prime_array_length_ > number)
		{
			return hash_helpers::kmax_prime_array_length_;
		}

		return hash_helpers::get_prime(num);
	}
}
