#pragma once

#include <cstdint>
#include <rax/shared.hpp>
#include <rax/math/math.hpp>

namespace rax::numerics
{
	template <typename T, std::uint32_t size> struct vector
	{
	private:
		T values_[size]{};

	public:
		RAX_INLINE vector()
		{
		}
		RAX_INLINE vector(const vector& other)
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				this->values_[i] = other.values_[i];
			}
		}
		RAX_INLINE vector& operator=(const vector& other)
		{
			if (this != &other)
			{
				for (std::uint32_t i = 0u; i < size; ++i)
				{
					this->values_[i] = other.values_[i];
				}
			}

			return *this;
		}

		RAX_INLINE auto operator[](std::uint32_t index) -> T&
		{
			return this->values_[index];
		}
		RAX_INLINE auto operator[](std::int32_t index) -> T&
		{
			return this->values_[index];
		}

		RAX_INLINE bool operator==(const vector& other)
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				if (this->values_[i] != other.values_[i])
				{
					return false;
				}
			}

			return true;
		}
		RAX_INLINE bool operator!=(const vector& other)
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				if (this->values_[i] != other.values_[i])
				{
					return true;
				}
			}

			return false;
		}
		RAX_INLINE auto operator+(const vector& other) -> vector
		{
			vector result{};

			for (std::uint32_t i = 0u; i < size; ++i)
			{
				result.values_[i] = this->values_[i] + other.values_[i];
			}

			return result;
		}
		RAX_INLINE auto operator-(const vector& other) -> vector
		{
			vector result{};

			for (std::uint32_t i = 0u; i < size; ++i)
			{
				result.values_[i] = this->values_[i] - other.values_[i];
			}

			return result;
		}
		RAX_INLINE auto operator*(const T& value) -> vector
		{
			vector result{};

			for (std::uint32_t i = 0u; i < size; ++i)
			{
				result.values_[i] = this->values_[i] * value;
			}

			return result;
		}
		RAX_INLINE auto operator/(const T& value) -> vector
		{
			vector result{};

			for (std::uint32_t i = 0u; i < size; ++i)
			{
				result.values_[i] = this->values_[i] / value;
			}

			return result;
		}
		RAX_INLINE auto operator+=(const vector& other) -> vector&
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				this->values_[i] += other.values_[i];
			}

			return *this;
		}
		RAX_INLINE auto operator-=(const vector& other) -> vector&
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				this->values_[i] += other.values_[i];
			}

			return *this;
		}
		RAX_INLINE auto operator*=(const T& value) -> vector&
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				this->values_[i] *= value;
			}

			return *this;
		}
		RAX_INLINE auto operator/=(const T& value) -> vector&
		{
			for (std::uint32_t i = 0u; i < size; ++i)
			{
				this->values_[i] /= value;
			}

			return *this;
		}
		RAX_INLINE auto operator-() -> vector
		{
			vector result{};

			for (std::uint32_t i = 0u; i < size; ++i)
			{
				result.values_[i] = -this->values_[i];
			}

			return result;
		}

		RAX_INLINE operator T*()
		{
			return reinterpret_cast<T*>(&this->values_[0]);
		}

		RAX_INLINE auto length() const -> T
		{
			return rax::math::sqrt(this->length_squared());
		}
		auto length_squared() const -> T
		{
			T result{};
			
			for (std::uint32_t i = 0; i < size; ++i)
			{
				result += this->values_[i] * this->values_[i];
			}

			return result;
		}
	};
}
