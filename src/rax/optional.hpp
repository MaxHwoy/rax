#pragma once

namespace rax
{
	/// <summary>
	/// Optionally contains a value.
	/// </summary>
	/// <typeparam name="T">The type of the value.</typeparam>
	template <typename T> class optional
	{
	public:
		RAX_INLINE optional() : value_(), has_value_()
		{

		}

		RAX_INLINE optional(const T& value) : value_(value), has_value_(true)
		{

		}

		RAX_INLINE bool has_value() const
		{
			return this->operator bool();
		}

		RAX_INLINE auto value() const -> const T&
		{
			return this->value_;
		}

		RAX_INLINE auto value() -> T&
		{
			return this->value_;
		}

		RAX_INLINE operator bool() const
		{
			return this->has_value_;
		}

	private:
		T value_;
		bool has_value_;
	};
}
