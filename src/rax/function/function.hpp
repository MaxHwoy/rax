#pragma once

namespace rax
{
	template <typename TResult> using function = TResult(*)();

	template <typename T, typename TResult> using function1 = TResult(*)(T);

	template <typename T1, typename T2, typename TResult> using function2 = TResult(*)(T1, T2);

	template <typename T1, typename T2, typename T3, typename TResult> using function3 = TResult(*)(T1, T2, T3);

	template <typename T1, typename T2, typename T3, typename T4, typename TResult> using function4 = TResult(*)(T1, T2, T3, T4);
}
