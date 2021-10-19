#pragma once

#include <cstdint>
#include <memory>
#include <rax/shared.hpp>
#include <rax/allocator.hpp>

namespace rax
{
	template <typename T> class refptr
	{
	private:
		T* ptr_;

		RAX_INLINE refptr()
		{
			auto data = ::calloc(sizeof(T) + sizeof(std::uint32_t), 1u);

			this->ptr_ = reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(data) + sizeof(std::uint32_t));

			this->increment_ref();
		}

		RAX_INLINE void increment_ref()
		{
			// reference counter is located right before stored pointer
			++(*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(this->ptr_) - sizeof(std::uint32_t)));
		}
		RAX_INLINE void decrement_ref()
		{
			// reference counter is located right before stored pointer
			--(*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(this->ptr_) - sizeof(std::uint32_t)));
		}
		RAX_INLINE auto reference_num() const -> std::uint32_t
		{
			return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(this->ptr_) - sizeof(std::uint32_t));
		}
		RAX_INLINE void destroy_value()
		{
			if (this->ptr_ == nullptr)
			{
				return;
			}

			this->decrement_ref();

			auto refcount = this->reference_num();

			if (refcount == 0u)
			{
				this->ptr_->~T();

				::free(reinterpret_cast<std::uint8_t*>(this->ptr_) - sizeof(std::uint32_t));

				this->ptr_ = nullptr;
			}
		}

	public:

		refptr(const refptr& other)
		{
			this->ptr_ = other.ptr_;
			this->increment_ref();
		}

		refptr(refptr&& other) noexcept
		{
			this->ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}

		refptr& operator=(const refptr& other)
		{
			if (this != &other)
			{
				this->destroy_value();

				this->ptr_ = other.ptr_;

				this->increment_ref();
			}

			return *this;
		}

		~refptr()
		{
			this->destroy_value();
		}

		RAX_INLINE auto operator*() -> T&
		{
			return *this->ptr_;
		}

		RAX_INLINE auto operator*() const -> const T&
		{
			return *this->ptr_;
		}

		RAX_INLINE auto operator->() -> T*
		{
			return this->ptr_;
		}

		RAX_INLINE auto operator->() const -> const T*
		{
			return this->ptr_;
		}

		RAX_INLINE bool operator==(const refptr& other)
		{
			return this->ptr_ == other.ptr_;
		}

		static auto make_ref() -> refptr
		{
			auto result = refptr();

			__init(result.ptr_) T();

			return result;
		}

		template <typename... Args> static auto make_ref(Args&&... args) -> refptr
		{
			auto result = refptr();

			__init(result.ptr_) T(std::forward<Args>(args)...);

			return result;
		}

		RAX_INLINE static auto get_refcount(const refptr& ptr) -> std::uint32_t
		{
			return ptr.reference_num();
		}
	};
}
