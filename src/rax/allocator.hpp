#pragma once

#include <cstdint>
#include <memory>
#include <rax/shared.hpp>

namespace rax
{
	class allocator final
	{
	public:
		using alloc_one = void*(*)(std::uint32_t size);
		using alloc_lot = void*(*)(std::uint32_t size, std::uint32_t count);
		using alloc_one_no_init = void*(*)(std::uint32_t size);
		using alloc_lot_no_init = void*(*)(std::uint32_t size, std::uint32_t count);

	private:
		static allocator default_;
		static allocator current_;

		alloc_one alloc_one_;
		alloc_lot alloc_lot_;
		alloc_one_no_init alloc_one_no_init_;
		alloc_lot_no_init alloc_lot_no_init_;

		static auto default_alloc_one(std::uint32_t size) -> void*;
		static auto default_alloc_lot(std::uint32_t size, std::uint32_t count) -> void*;
		static auto default_alloc_one_no_init(std::uint32_t size) -> void*;
		static auto default_alloc_lot_no_init(std::uint32_t size, std::uint32_t count) -> void*;

	public:
		allocator();

		RAX_INLINE static auto get_default() -> const allocator&
		{
			return allocator::default_;
		}
		RAX_INLINE static auto get_current() -> const allocator&
		{
			return allocator::current_;
		}
		RAX_INLINE static void set_current(const allocator& alloc)
		{
			allocator::current_ = alloc;
		}
	};
}
