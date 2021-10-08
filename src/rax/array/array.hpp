#pragma once

namespace rax
{
	class array_debug_info final
	{
	public:
		static inline std::int32_t num_allocated_arrays = 0;
		static inline std::int32_t num_destroyed_arrays = 0;
	};

	template <typename T> class array
	{
	private:
		static array empty_;
		
		std::uint8_t* ptr_;
		std::uint32_t size_;

		RAX_INLINE void increment_ref()
		{
			// reference counter is located right before stored pointer
			++(*reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint8_t*)));
		}
		RAX_INLINE void decrement_ref()
		{
			// reference counter is located right before stored pointer
			--(*reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint8_t*)));
		}
		RAX_INLINE auto reference_num() -> std::uint32_t
		{
			return *reinterpret_cast<std::uint32_t*>(this->ptr_ - sizeof(std::uint8_t*));
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
				::printf("Destroying array of size %d\n", this->size_);
#endif // !DEBUG_ARRAY_PRINT

				::free(this->ptr_ - sizeof(std::uint8_t*));

				this->ptr_ = nullptr;
				this->size_ = 0u;

				++array_debug_info::num_destroyed_arrays;
			}
		}

	public:
		array() : array(0u)
		{
		}
		array(std::uint32_t size)
		{
			// minimum allocation size will always be size of pointer
			// that is used for reference counting

#ifdef DEBUG_ARRAY_PRINT
			::printf("Allocating array of size %d\n", size);
#endif // !DEBUG_ARRAY_PRINT

			auto data = ::calloc(sizeof(void*) + size * sizeof(T), 1u);

			this->ptr_ = reinterpret_cast<std::uint8_t*>(data) + sizeof(std::uint8_t*);
			this->size_ = size;

			this->increment_ref();

			++array_debug_info::num_allocated_arrays;
		}
		array(const array& other)
		{
			// when copying we just increment reference
			this->ptr_ = other.ptr_;
			this->size_ = other.size_;

			this->increment_ref();
		}
		array(array&& other) noexcept
		{
			// when moving we remove one reference and add one
			// at the same time; we also assign nullptr to the
			// previous array so we skip its destruction later

			this->ptr_ = other.ptr_;
			this->size_ = other.size_;
			other.ptr_ = nullptr;
			other.size_ = 0u;
		}
		array& operator=(const array& other)
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
				this->size_ = other.size_;

				this->increment_ref();
			}

			return *this;
		}
		~array()
		{
			this->destroy_array();
		}

		RAX_INLINE auto operator[](std::int32_t index) -> T&
		{
			assert(0 <= index && index < this->size_);
			return reinterpret_cast<T*>(this->ptr_)[static_cast<std::uint32_t>(index)];
		}
		RAX_INLINE auto operator[](std::uint32_t index) -> T&
		{
			assert(index < this->size_);
			return reinterpret_cast<T*>(this->ptr_)[index];
		}

		RAX_INLINE auto length() const -> std::uint32_t
		{
			return this->size_;
		}
		
		static auto empty() -> const array&
		{
			return array::empty_;
		}
		static void resize(array& arr, std::uint32_t new_size)
		{
			if (new_size != arr.size_)
			{
				if (new_size == 0u)
				{
					arr = array(0u);
					return;
				}

				if (arr.size_ == 0u)
				{
					arr = array(new_size);
					return;
				}

				auto result = array(new_size);
				auto length = (new_size > arr.size_ ? arr.size_ : new_size) * sizeof(T);

				::memmove(result.ptr_, arr.ptr_, length);

				arr = result;
			}
		}
	};

	template <typename T> array<T> array<T>::empty_ = array<T>(0u);
}
