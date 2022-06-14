#pragma once

#include <cstdint>
#include <memory>
#include <rax/shared.hpp>

namespace rax
{
	class allocator
	{
	public:
		allocator() = default;

		template <typename T> RAX_INLINE auto allocate() const -> T*
		{
			return reinterpret_cast<T*>(::calloc(1u, sizeof(T)));
		}
		template <typename T> RAX_INLINE auto allocate_no_init() const -> T*
		{
			return reinterpret_cast<T*>(::malloc(sizeof(T)));
		}

		RAX_INLINE auto allocate(size_t size) const -> void*
		{
			return ::calloc(size, 1u);
		}
		RAX_INLINE auto allocate(size_t size, size_t count) const -> void*
		{
			return ::calloc(count, size);
		}
		RAX_INLINE auto allocate_no_init(size_t size) const -> void*
		{
			return ::malloc(size);
		}
		RAX_INLINE auto allocate_no_init(size_t size, size_t count) const -> void*
		{
			return ::malloc(size * count);
		}

		RAX_INLINE void free(void* ptr) const
		{
			::free(ptr);
		}
	};
}
