#pragma once

namespace rax
{
	template <typename T> class array
	{
	private:
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
			// // this happens only when this array was moved prior
			// or if this array has already been destroyed manually
			if (this->ptr_ == nullptr)
			{
				return;
			}

			// decrement reference
			this->decrement_ref();

			auto refcount = this->reference_num();

			// if no references exists, destroy array
			if (refcount == 0u)
			{
				::printf("Destroying array of size %d\n", this->size_);

				::free(this->ptr_ - sizeof(std::uint8_t*));

				this->ptr_ = nullptr;
				this->size_ = 0u;
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

			auto data = ::calloc(sizeof(void*) + size * sizeof(T), 1u);

			this->ptr_ = reinterpret_cast<std::uint8_t*>(data) + sizeof(std::uint8_t*);
			this->size_ = size;

			increment_ref();

			::printf("Allocation array of size %d\n", size);
		}
		array(const array& other)
		{
			// when copying we just increment reference
			this->ptr_ = other.ptr_;
			this->size_ = other.size_;

			increment_ref();
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

				// otherwise increment reference count
				this->ptr_ = other.ptr_;
				this->size_ = other.size_;

				increment_ref();
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
}
