#pragma once

namespace rax
{
	using action = void(*)();

	template <typename T> using action1 = void(*)(T);

	template <typename T1, typename T2> using action2 = void(*)(T1, T2);

	template <typename T1, typename T2, typename T3> using action3 = void(*)(T1, T2, T3);

	template <typename T1, typename T2, typename T3, typename T4> using action4 = void(*)(T1, T2, T3, T4);

	template <typename T> using comparison = std::int32_t(*)(T, T);
	
	template <typename T, typename U> using converter = U(*)(T);
	
	template <typename T> using predicate = bool(*)(T);
}
