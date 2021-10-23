#pragma once

namespace rax
{
	/// <summary>
	/// Calls a Callable upon destruction.
	/// </summary>
	/// <typeparam name="F">Any Callable. (a type for which the invoke operation is defined)</typeparam>
	template <typename F> class finally
	{
	public:
		RAX_INLINE finally(F callable) : callable_(std::move(callable)), is_engaged_(true)
		{

		}

		RAX_INLINE finally(finally&& other) : callable_(std::move(other.callable_)), is_engaged_(other.is_engaged_)
		{
			other.disengage();
		}

		RAX_INLINE ~finally()
		{
			if (this->is_engaged_)
			{
				this->callable_();
			}
		}

		RAX_INLINE void disengage()
		{
			this->is_engaged_ = false;
		}

	private:
		RAX_NO_COPY(finally);

		finally() = delete;
		auto operator=(finally&&) -> finally& = delete;

		F callable_;
		bool is_engaged_;
	};
}
