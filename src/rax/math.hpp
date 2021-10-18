#pragma once

#include <cstdint>
#include <stdarg.h>
#include <immintrin.h>
#include <intrin.h>

#include <rax/shared.hpp>
#include <rax/tuple.hpp>

namespace rax
{
	class math final
	{
	private:
		static auto mod_power(std::uint32_t a, std::uint32_t n, std::uint32_t mod) -> std::uint64_t
		{
			std::uint64_t power = a, result = 1u;

			while (n)
			{
				if (n & 1)
				{
					result = (result * power) % mod;
				}

				power = (power * power) % mod;
				n >>= 1;
			}

			return result;
		}
		static bool mod_witness(std::uint32_t a, std::uint32_t n)
		{
			std::uint32_t u = n >> 1;
			std::uint32_t t = 1u;

			while (!(u & 1))
			{
				u >>= 1;
				++t;
			}

			std::uint64_t prev = mod_power(a, u, n);
			std::uint64_t curr = 0u;

			for (std::uint32_t i = 1u; i <= t; ++i)
			{
				curr = (prev * prev) % n;
				
				if ((curr == 1) && (prev != 1) && (prev != n - 1))
				{
					return true;
				}

				prev = curr;
			}

			if (curr != 1)
			{
				return true;
			}

			return false;
		}

	public:
		template <typename T> RAX_INLINE static auto abs(T value) -> T
		{
			if (value < 0)
			{
				return -value;
			}

			return value;
		}

		template <typename T> RAX_INLINE auto clamp(T value, T min, T max) -> T
		{
			value = (value < min) ? min : value;
			value = (value > max) ? max : value;

			return value;
		}

		static bool is_prime(std::uint32_t number)
		{
			// https://stackoverflow.com/questions/4424374/determining-if-a-number-is-prime
			// LostInTheCode 2010

			if (((!(number & 1u)) && number != 2u) || (number < 2u) || (number % 3u == 0 && number != 3u))
			{
				return false;
			}

			if (number < 1373653u)
			{
				for (std::uint32_t k = 1u; 36u * k * k - 12u * k < number; ++k)
				{
					if ((number % (6u * k + 1u) == 0) || (number % (6u * k - 1u) == 0))
					{
						return (false);
					}
				}

				return true;
			}

			if (number < 9080191u)
			{
				if (math::mod_witness(31u, number))
				{
					return false;
				}
				
				if (math::mod_witness(73u, number))
				{
					return false;
				}

				return true;
			}

			if (math::mod_witness(2u, number))
			{
				return false;
			}

			if (math::mod_witness(7u, number))
			{
				return false;
			}
			
			if (math::mod_witness(61u, number))
			{
				return false;
			}

			return true;
		}

		template <typename T> RAX_INLINE static auto max(T a, T b) -> T
		{
			return a > b ? a : b;
		}
		template <typename T> RAX_INLINE static auto max(T a, T b, T c) -> T
		{
			if (a > b)
			{
				return a > c ? a : c;
			}
			else
			{
				return b > c ? b : c;
			}
		}
		template <typename T> static auto max(T* ptr, std::uint32_t size) -> T
		{
			if (size == 0)
			{
				return T{};
			}

			auto result = ptr[0];

			for (std::uint32_t i = 1u; i < size; ++i)
			{
				if (ptr[i] > result)
				{
					result = ptr[i];
				}
			}

			return result;
		}
		template <typename T> static auto max(std::uint32_t count, ...) -> T
		{
			if (count == 0)
			{
				return T{};
			}

			va_list va;
			va_start(va, count);

			auto result = va_arg(va, T);

			for (std::uint32_t i = 1u; i < count; ++i)
			{
				auto value = va_arg(va, T);

				if (value > result)
				{
					result = value;
				}
			}

			va_end(va);

			return result;
		}
		
		template <typename T> RAX_INLINE static auto min(T a, T b) -> T
		{
			return a < b ? a : b;
		}
		template <typename T> RAX_INLINE static auto min(T a, T b, T c) -> T
		{
			if (a < b)
			{
				return a < c ? a : c;
			}
			else
			{
				return b < c ? b : c;
			}
		}
		template <typename T> static auto min(T* ptr, std::uint32_t size) -> T
		{
			if (size == 0)
			{
				return T{};
			}

			auto result = ptr[0];

			for (std::uint32_t i = 1u; i < size; ++i)
			{
				if (ptr[i] < result)
				{
					result = ptr[i];
				}
			}

			return result;
		}
		template <typename T> static auto min(std::uint32_t count, ...) -> T
		{
			if (count == 0)
			{
				return T{};
			}

			va_list va;
			va_start(va, count);

			auto result = va_arg(va, T);

			for (std::uint32_t i = 1u; i < count; ++i)
			{
				auto value = va_arg(va, T);

				if (value < result)
				{
					result = value;
				}
			}

			va_end(va);

			return result;
		}

		template <typename T> RAX_INLINE static auto minmax(T a, T b) -> decltype(auto)
		{
			return a < b ? tuple2<T, T>(a, b) : tuple2<T, T>(b, a);
		}
		template <typename T> RAX_INLINE static auto minmax(T a, T b, T c) -> decltype(auto)
		{
			auto pair = minmax(a, b);
			
			return tuple2(min(c, pair.item1()), max(c, pair.item2()));
		}
		template <typename T> static auto minmax(T* ptr, std::uint32_t size) -> decltype(auto)
		{
			if (size == 0)
			{
				return tuple2(T{}, T{});
			}

			auto max = ptr[0];
			auto min = max;
			
			for (std::uint32_t i = 1u; i < size; ++i)
			{
				auto value = ptr[i];

				if (value < min)
				{
					min = value;
				}
				else if (value > max)
				{
					max = value;
				}
			}

			return tuple2(min, max);
		}
		template <typename T> static auto minmax(std::uint32_t count, ...) -> decltype(auto)
		{
			if (count == 0)
			{
				return tuple2(T{}, T{});
			}

			va_list va;
			va_start(va, count);

			auto max = va_arg(va, T);
			auto min = max;

			for (std::uint32_t i = 1u; i < count; ++i)
			{
				auto value = va_arg(va, T);

				if (value < min)
				{
					min = value;
				}
				else if (value > max)
				{
					max = value;
				}
			}

			va_end(va);

			return tuple2(min, max);
		}
		
		static auto sqrt(float value) -> float
		{
			const auto result = ::_mm_sqrt_ss(::_mm_load_ss(&value));

			return result.m128_f32[0];
		}
	};
}
