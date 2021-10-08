#pragma once

namespace rax
{
	class math final
	{
	public:
		template <typename T> static auto abs(T value) -> T
		{
			if (value < 0)
			{
				return -value;
			}

			return value;
		}

		template <typename T> static auto max(T a, T b) -> T
		{
			return a > b ? a : b;
		}
		template <typename T> static auto max(T a, T b, T c) -> T
		{
			if (a > b)
			{
				return a > c ? a : c;
			}
			else
			{
				return a > c ? a : c;
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
		
		template <typename T> static auto min(T a, T b) -> T
		{
			return a < b ? a : b;
		}
		template <typename T> static auto min(T a, T b, T c) -> T
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

		template <typename T> static auto minmax(T a, T b) -> decltype(auto)
		{
			return a < b ? tuple2<T, T>(a, b) : tuple2<T, T>(b, a);
		}
		template <typename T> static auto minmax(T a, T b, T c) -> decltype(auto)
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

		static auto sqrt(float value) -> float;
	};
}
