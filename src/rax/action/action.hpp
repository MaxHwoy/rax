#pragma once

#include <cstdint>

namespace rax
{
	using action = void(*)();

	template <typename T> using action1 = void(*)(T* obj);

	template <typename T1, typename T2> using action2 = void(*)(T1* arg1, T2* arg2);

	template <typename T1, typename T2, typename T3> using action3 = void(*)(T1* arg1, T2* arg2, T3* arg3);

	template <typename T1, typename T2, typename T3, typename T4> using action4 = void(*)(T1* arg1, T2* arg2, T3* arg3, T4* arg4);

	/// <summary>
	/// Compares two objects. An implementation of this method must return a value less than zero if
	/// x is less than y, zero if x is equal to y, or a value greater than zero if x is greater than y.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T> using comparison = std::int32_t(*)(const T* x, const T* y);
	
	template <typename TInput, typename TOutput> using converter = TOutput(*)(const TInput* input);
	
	template <typename T> using predicate = bool(*)(const T* obj);
}
