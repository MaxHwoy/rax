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
		RAX_INLINE finally(F&& callable) : callable_(callable)
		{

		}

		RAX_INLINE ~finally()
		{
			callable_();
		}

	private:
		F callable_;
	};
}
