#include <rax/allocator.hpp>

namespace rax
{
	auto allocator::default_alloc_one(std::uint32_t size) -> void*
	{
		return ::calloc(1u, size);
	}

	auto allocator::default_alloc_lot(std::uint32_t size, std::uint32_t count) -> void*
	{
		return ::calloc(count, size);
	}
	
	auto allocator::default_alloc_one_no_init(std::uint32_t size) -> void*
	{
		return ::malloc(size);
	}
	
	auto allocator::default_alloc_lot_no_init(std::uint32_t size, std::uint32_t count) -> void*
	{
		return ::malloc(size * count);
	}

	allocator::allocator()
	{

	}

	allocator allocator::default_ = allocator();
}
