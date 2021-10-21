#pragma once

#include <rax/shared.hpp>

#include <cstdint>
#include <type_traits>

namespace rax
{
	template <typename T> class span
	{
	public:
		using element_type = T;
		using value_type = std::remove_cv<T>;
		using size_type = std::size_t;

		RAX_INLINE span() : span<T>(nullptr, 0)
		{

		}

		template <std::size_t N> RAX_INLINE span(T(&array)[N]) : span<T>(reinterpret_cast<T*>(array), N)
		{

		}

		RAX_INLINE span(T* data, std::size_t size) : data_(reinterpret_cast<std::uint8_t*>(data)), size_(size)
		{

		}

		RAX_INLINE auto begin() -> T*
		{
			return reinterpret_cast<T*>(this->data_);
		}

		RAX_INLINE auto begin() const -> const T*
		{
			return reinterpret_cast<T*>(this->data_);
		}

		RAX_INLINE auto end() -> T*
		{
			return reinterpret_cast<T*>(this->data_ + this->size_);
		}

		RAX_INLINE auto end() const -> const T*
		{
			return reinterpret_cast<T*>(this->data_ + this->size_);
		}

		RAX_INLINE auto size() const -> std::size_t
		{
			return this->size_;
		}

		RAX_INLINE auto subspan(std::size_t offset) const -> span<T>
		{
			if (this->size_ > offset)
			{
				return span<T>(this->data_ + offset, this->size_ - offset);
			}

			return {};
		}

	private:
		std::uint8_t* data_;
		std::size_t size_;
	};
}
