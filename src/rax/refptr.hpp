#pragma once

#include <cstdint>
#include <memory>
#include <rax/shared.hpp>
#include <rax/allocator.hpp>

namespace rax
{
	template <typename T, bool thread_safe = false, typename alloc = allocator> class refptr
	{
	private:
		static_assert(sizeof(std::uint32_t) == sizeof(std::atomic_uint32_t));

		static constexpr auto ptr_offset = sizeof(std::uint32_t);
		static constexpr auto ref_offset = sizeof(std::uint32_t);

		T* ptr_;

		RAX_INLINE refptr()
		{
			auto data = alloc().allocate_no_init(sizeof(T) + ptr_offset);

			this->ptr_ = reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(data) + ptr_offset);

			this->increment_ref();
		}

		RAX_INLINE void increment_ref()
		{
			// reference counter is located right before stored pointer
			if constexpr (thread_safe)
			{
				++(*reinterpret_cast<std::atomic_uint32_t*>(this->ptr_ - ref_offset));
			}
			else
			{
				++(*reinterpret_cast<std::uint32_t*>(this->ptr_ - ref_offset));
			}
		}
		RAX_INLINE void decrement_ref()
		{
			// reference counter is located right before stored pointer
			if constexpr (thread_safe)
			{
				--(*reinterpret_cast<std::atomic_uint32_t*>(this->ptr_ - ref_offset));
			}
			else
			{
				--(*reinterpret_cast<std::uint32_t*>(this->ptr_ - ref_offset));
			}
		}
		RAX_INLINE auto reference_num() const -> std::uint32_t
		{
			// reference counter is located right before stored pointer
			if constexpr (thread_safe)
			{
				return *reinterpret_cast<std::atomic_uint32_t*>(this->ptr_ - ref_offset);
			}
			else
			{
				return *reinterpret_cast<std::uint32_t*>(this->ptr_ - ref_offset);
			}
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

				alloc().free(reinterpret_cast<std::uint8_t*>(this->ptr_) - ptr_offset);

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
				// this might be a reassignment, hence decrement reference of the current object
				this->destroy_value();

				// assign new pointer to the current one
				this->ptr_ = other.ptr_;

				// increment reference counter after assignment
				this->increment_ref();
			}

			return *this;
		}

		refptr& operator=(refptr&& other) noexcept
		{
			this->ptr_ = other.ptr_;
			other.ptr_ = nullptr;
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

#define __makeref(T) rax::refptr<T>::make_ref()
#define __makerefsafe(T) rax::refptr<T, true>::make_ref()
