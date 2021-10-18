#pragma once

#define null nullptr

#ifdef _MSC_VER
#define RAX_INLINE __forceinline
#else
#define RAX_INLINE __attribute__((always_inline)) inline
#endif

#define RAX_NO_INLINE __declspec(noinline)

#define RAX_NO_INTERFACE(T) class __declspec(novtable) T abstract

#define RAX_NO_COPY(T)								\
	T(const T&) = delete;							\
	auto operator=(const T&) -> T& = delete;		\
	auto operator=(const T&) volatile -> T& = delete

#define RAX_ASSERT_SIZE(T, N) static_assert(sizeof(T) == N, "sizeof("#T") != "#N)

#define REF(T) const T&

// this is used specifically for evaluating current targetted OS
// __LP64__ is defined for GCC
// _M_X64 is defined for MSVC
// __aarch64__ is defined for Clang
#if defined(__LP64__) || defined(_M_X64) || defined(__aarch64__)
#define TARGET_64BIT
#endif // defined(__LP64__) || defined(_M_X64) || defined(__aarch64__)

// defines current platform to be Win32
#if defined(_WIN32)
#define PLATFORM_WIN32
#endif // defined(_WIN32)

// defines current platform to be Win64; note that _WIN32 is defined when _WIN64 is defined
#if defined(_WIN64)
#define PLATFORM_WIN64
#endif // defined(_WIN64)

// defines current platform to be Linux
#if defined(__linux__)
#define PLATFORM_LINUX
#endif // defined(__linux__)

// defines current platform to be OS-X
#if defined(__APPLE__)
#define PLATFORM_OSX
#endif // defined(__APPLE__)

// defines current platform to be Unix
#if defined(__unix__)
#define PLATFORM_UNIX
#endif // defined(__unix__)

// defines current platform to be Windows
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
#define PLATFORM_WINDOWS
#endif

// non inline intrinsics are faster
// #ifdef _MSC_VER
// #pragma function(memcpy, memcmp, memset, strcmp, strcpy, strlen, strcat)
// #pragma intrinsic(memcmp)
// #pragma intrinsic(memcpy)
// #pragma intrinsic(memset)
// #pragma intrinsic(strcmp)
// #pragma intrinsic(strcpy)
// #pragma intrinsic(strlen)
// #pragma intrinsic(strcat)
// #endif  // defined(_MSC_VER)

// debug macros
// #define DEBUG_ARRAY_PRINT
