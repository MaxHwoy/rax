#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include <algorithm>
#include <rax/shared.hpp>
#include <rax/action/action.hpp>
#include <rax/arraysort/arraysort.hpp>

namespace rax
{
	class refarray_debug_info final
	{
	public:
		static inline std::int32_t num_allocated_refarrays = 0;
		static inline std::int32_t num_destroyed_refarrays = 0;
	};

	template <typename T> class refarray
	{
	private:
		static refarray empty_;

		// size of refarray should equal exactly one pointer
		// so it can be passed by value in any function call
		// reference counter is located at offset ptr_ - 4
		// array length is located at offset ptr_ - 8
		std::uint8_t* ptr_;

		RAX_INLINE void increment_ref()
		{
			// reference counter is located right before stored pointer
			++(*reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint32_t)));
		}
		RAX_INLINE void decrement_ref()
		{
			// reference counter is located right before stored pointer
			--(*reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint32_t)));
		}
		RAX_INLINE auto reference_num() -> std::uint32_t
		{
			return *reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint32_t));
		}
		RAX_INLINE void destroy_array()
		{
			// if already nullptr, no need to do stuff further
			// this happens only when this array was moved prior
			// or if this array has already been destroyed manually
			if (this->ptr_ == nullptr)
			{
				return;
			}

			// decrement reference
			this->decrement_ref();

			// get reference counter
			auto refcount = this->reference_num();

			// if no references exists, destroy array
			if (refcount == 0u)
			{
#ifdef DEBUG_ARRAY_PRINT
				::printf("Destroying ref array of size %d\n", this->length());
#endif // !DEBUG_ARRAY_PRINT

				::free(this->ptr_ - sizeof(std::uint64_t));

				this->ptr_ = nullptr;

				++refarray_debug_info::num_destroyed_refarrays;
			}
		}

	public:
		refarray() : refarray(0u)
		{
		}
		refarray(std::uint32_t size)
		{
			// minimum allocation size will always be 8 bytes where
			// 4 bytes are used for length and other 4 for ref count

#ifdef DEBUG_ARRAY_PRINT
			::printf("Allocating ref array of size %d\n", size);
#endif // !DEBUG_ARRAY_PRINT

			// allocate element storage with minimum size of 8 bytes
			auto data = ::calloc(sizeof(std::uint64_t) + size * sizeof(T), 1u);

			// store size of the array at the given pointer
			*reinterpret_cast<std::uint32_t*>(data) = size;

			// ptr_ will be pointing right after array size and ref counter
			this->ptr_ = reinterpret_cast<std::uint8_t*>(data) + sizeof(std::uint64_t);

			// set initial reference counter to 1
			this->increment_ref();

			++refarray_debug_info::num_allocated_refarrays;
		}
		refarray(const refarray& other)
		{
			// when copying we just increment reference
			this->ptr_ = other.ptr_;
			this->increment_ref();
		}
		refarray(refarray&& other) noexcept
		{
			// when moving we remove one reference and add one
			// at the same time; we also assign nullptr to the
			// previous array so we skip its destruction later

			this->ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}
		refarray& operator=(const refarray& other)
		{
			// if we assign to itself, no need in incrementing
			if (this != &other)
			{
				// try destroy this array in case its
				// reference count will be 0
				this->destroy_array();

				// after that assign new pointer and size
				// and increment reference counter
				this->ptr_ = other.ptr_;

				// increment reference counter of the new
				// array received
				this->increment_ref();
			}

			return *this;
		}
		~refarray()
		{
			this->destroy_array();
		}

		//
		// instance
		//
		RAX_INLINE auto operator[](std::int32_t index) -> T&
		{
			return reinterpret_cast<T*>(this->ptr_)[static_cast<std::uint32_t>(index)];
		}
		RAX_INLINE auto operator[](std::uint32_t index) -> T&
		{
			return reinterpret_cast<T*>(this->ptr_)[index];
		}

		RAX_INLINE auto get_value(std::uint32_t index) const -> const T&
		{
			return reinterpret_cast<T*>(this->ptr_)[index];
		}
		RAX_INLINE void set_value(std::uint32_t index, const T& value)
		{
			reinterpret_cast<T*>(this->ptr_)[index] = value;
		}

		RAX_INLINE auto length() const -> std::uint32_t
		{
			return *reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint64_t));
		}

		RAX_INLINE auto start() -> T*
		{
			return reinterpret_cast<T*>(this->ptr_);
		}
		RAX_INLINE auto end() -> T*
		{
			return reinterpret_cast<T*>(this->ptr_) + this->length();
		}
		
		RAX_INLINE void memory_copy_to(REF(refarray) arr, std::uint32_t index)
		{
			refarray::memory_copy(this, 0u, arr, index, this->length());
		}
		RAX_INLINE void value_copy_to(REF(refarray) arr, std::uint32_t index)
		{
			refarray::value_copy(this, 0u, arr, index, this->length());
		}

		//
		// static
		//
		static auto binary_search(REF(refarray) arr, const T& value) -> std::int32_t
		{
			return refarray::binary_search(arr, 0u, arr.length(), value);
		}

		static auto binary_search(REF(refarray) arr, std::uint32_t start, std::uint32_t length, const T& value) -> std::int32_t
		{
			auto end = start + length;

			assert(end <= arr.length());

			auto middle = (start + end) >> 1;

			while (start <= end)
			{
				const auto& ref = arr.get_value(middle);

				if (value == ref)
				{
					return middle;
				}

				if (value < ref)
				{
					end = middle - 1;
				}
				else
				{
					start = middle + 1;
				}

				middle = (start + end) >> 1;
			}

			return -1;
		}

		static auto binary_search(REF(refarray) arr, const T& value, comparison<T> comparer) -> std::int32_t
		{
			return refarray::binary_search(arr, 0u, arr.length(), value, comparer);
		}

		static auto binary_search(REF(refarray) arr, std::uint32_t start, std::uint32_t length, const T& value, comparison<T> comparer) -> std::int32_t
		{
			auto end = start + length;
			
			assert(end <= arr.length());

			auto middle = (start + end) >> 1;

			if (comparer == nullptr)
			{
				return -1;
			}

			while (start <= end)
			{
				auto comp = comparer(value, &arr.get_value(middle));

				if (comp == 0)
				{
					return middle;
				}

				if (comp < 0)
				{
					end = middle - 1;
				}
				else
				{
					start = middle + 1;
				}

				middle = (start + end) >> 1;
			}

			return -1;
		}

		template <typename TOutput> static auto convert_all(REF(refarray) arr, converter<T, TOutput> convert) -> refarray<TOutput>
		{
			assert(convert != nullptr);

			auto length = arr.length();
			auto result = refarray<TOutput>(length);

			for (std::uint32_t i = 0u; i < length; ++i)
			{
				result.set_value(i, convert(&arr.get_value(i)));
			}

			return result;
		}

		static auto empty() -> REF(refarray)
		{
			return refarray::empty_;
		}
		
		static bool exists(REF(refarray) arr, predicate<T> match)
		{
			return refarray::find_index(arr, 0u, arr.length(), match) != -1;
		}

		// FILL

		// FIND

		static auto find_index(REF(refarray) arr, predicate<T> match) -> std::int32_t
		{
			return refarray::find_index(arr, 0u, arr.length(), match);
		}

		static auto find_index(REF(refarray) arr, std::uint32_t start, predicate<T> match) -> std::int32_t
		{
			return refarray::find_index(arr, start, arr.length(), match);
		}

		static auto find_index(REF(refarray) arr, std::uint32_t start, std::uint32_t length, predicate<T> match) -> std::int32_t
		{
			auto end = start + length;

			assert(match != nullptr);
			assert(end <= arr.length());

			for (std::uint32_t i = start; i < end; ++i)
			{
				if (match(&arr.get_value(i)))
				{
					return i;
				}
			}

			return -1;
		}

		static void memory_copy(REF(refarray) src_array, REF(refarray) dest_array, std::uint32_t length)
		{
			refarray::memory_copy(src_array, 0u, dest_array, 0u, length);
		}

		static void memory_copy(REF(refarray) src_array, std::uint32_t src_index, REF(refarray) dest_array, std::uint32_t dest_index, std::uint32_t length)
		{
			assert(src_index + length <= src_array.length());
			assert(dest_index + length <= dest_array.length());

			::memmove(dest_array.ptr_ + dest_index * sizeof(T), src_array.ptr_ + src_index * sizeof(T), length * sizeof(T));
		}

		static void resize(refarray& arr, std::uint32_t new_size)
		{
			auto length = arr.length();

			if (new_size != length)
			{
				if (new_size == 0u)
				{
					arr = refarray(0u);
					return;
				}

				if (length == 0u)
				{
					arr = refarray(new_size);
					return;
				}

				auto result = refarray(new_size);
				auto length = (new_size > length ? length : new_size) * sizeof(T);

				::memmove(result.ptr_, arr.ptr_, length);

				arr = result;
			}
		}

		
		// SORTS
		static void sort(REF(refarray) arr)
		{
			refarray::sort(arr, 0u, arr.length());
		}

		static void sort(REF(refarray) arr, comparison<T> comparer)
		{
			refarray::sort(arr, 0u, arr.length(), comparer);
		}

		static void sort(REF(refarray) arr, std::uint32_t start, std::uint32_t length)
		{
			if (length <= 1)
			{
				return; // why would we need to sort arrays of 0 or 1 elements?
			}

			auto end = start + length;

			assert(end <= arr.length());

			auto ptr = reinterpret_cast<T*>(arr.ptr_);

			arraysort<T>::introspective_sort(ptr, start, length);
		}

		static void sort(REF(refarray) arr, std::uint32_t start, std::uint32_t length, comparison<T> comparer)
		{
			if (length <= 1)
			{
				return; // why would we need to sort arrays of 0 or 1 elements?
			}

			auto end = start + length;

			assert(end <= arr.length());

			auto ptr = reinterpret_cast<T*>(arr.ptr_);

			arraysort<T>::introspective_sort(ptr, start, length, comparer);
		}

		static void value_copy(REF(refarray) src_array, REF(refarray) dest_array, std::uint32_t length)
		{
			refarray::value_copy(src_array, 0u, dest_array, 0u, length);
		}
		
		static void value_copy(REF(refarray) src_array, std::uint32_t src_index, REF(refarray) dest_array, std::uint32_t dest_index, std::uint32_t length)
		{
			assert(src_index + length <= src_array.length());
			assert(dest_index + length <= dest_array.length());

			for (std::uint32_t i = 0u; i < length; ++i)
			{
				dest_array.set_value(i + dest_index, src_array.get_value(i + src_index));
			}
		}
	};

	template <typename T> refarray<T> refarray<T>::empty_ = refarray<T>(0u);

	// we assert size of a reference array to be same as size of one pointer
	// that will be 4 bytes on 32-bit and 8 bytes on 64-bit systems
	RAX_ASSERT_SIZE(refarray<std::uint32_t>, sizeof(void*));
}
