#pragma once

#ifdef _MSC_VER
#define RAX_INLINE __forceinline
#else
#define RAX_INLINE __attribute__((always_inline)) inline
#endif

#define RAX_NO_INLINE __declspec(noinline)

#define RAX_NO_INTERFACE(T) class __declspec(novtable) T abstract

#define RAX_NO_COPY(T)                      \
	T(const T&) = delete;                   \
	auto operator=(const T&) -> T& = delete

// debug macros
#define DEBUG_ARRAY_PRINT
