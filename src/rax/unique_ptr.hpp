#pragma once

namespace rax
{
	template <typename T> class unique_ptr
	{
	public:
		RAX_INLINE unique_ptr() noexcept : unique_ptr<T>(reinterpret_cast<T*>(nullptr))
		{

		}

		RAX_INLINE unique_ptr(T* value) noexcept : value_(value)
		{

		}

		template <typename... Args> RAX_INLINE unique_ptr(Args&&... args)
			: unique_ptr<T>(new T(std::forward<Args>(args)...))
		{

		}

		RAX_INLINE unique_ptr(unique_ptr<T>&& other) noexcept : unique_ptr<T>(other.value_)
		{
			other.value_ = nullptr;
		}

		RAX_INLINE ~unique_ptr()
		{
			if (this->value_)
			{
				delete this->value_;

				this->value_ = nullptr;
			}
		}

		RAX_INLINE auto get() -> T*
		{
			return this->value_;
		}

		RAX_INLINE auto get() const -> const T*
		{
			return this->value_;
		}

		RAX_INLINE auto operator*() -> T&
		{
			return *this->value_;
		}

		RAX_INLINE auto operator*() const -> const T&
		{
			return *this->value_;
		}

		RAX_INLINE auto operator->() const -> const T*
		{
			return this->value_;
		}

		RAX_INLINE auto operator->() -> T*
		{
			return this->value_;
		}

		RAX_INLINE bool operator==(const unique_ptr<T>& other)
		{
			return this->value_ == other.value_;
		}

	private:
		RAX_NO_COPY(unique_ptr<T>);

		T* value_;
	};
}
