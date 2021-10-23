#include <rax/environment.hpp>

#include <thread>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif // PLATFORM_WINDOWS

#ifdef PLATFORM_UNIX
#include <unistd.h>
#endif // PLATFORM_UNIX

namespace rax
{
	auto environment::processor_count() -> std::uint32_t
	{
		// hardware_concurrency returns maximum number
		// of threads that can run concurrently, which
		// equals total number of processors
		return std::thread::hardware_concurrency();
	}

	bool environment::is_single_processor()
	{
		return environment::processor_count() == 1;
	}

	auto environment::get_environment_variable(const wchar_t* variable) -> const wchar_t*
	{
		::GetEnvironmentVariableW(variable, environment::internal_buffer_, sizeof(environment::internal_buffer_) / sizeof(wchar_t));

		return environment::internal_buffer_;
	}

	void environment::set_environment_variable(const char* variable, const char* value)
	{
#ifdef PLATFORM_WINDOWS
		::SetEnvironmentVariableA(variable, value);
#else
		// TODO
#endif // PLATFORM_WINDOWS
	}

	void environment::set_environment_variable(const wchar_t* variable, const wchar_t* value)
	{
#ifdef PLATFORM_WINDOWS
		::SetEnvironmentVariableW(variable, value);
#else
		// TODO
#endif // PLATFORM_WINDOWS
	}

	auto environment::process_id() -> std::uint32_t
	{
#ifdef PLATFORM_WINDOWS
		return ::GetCurrentProcessId();
#else
		return ::getpid();
#endif // PLATFORM_WINDOWS
	}

	auto environment::new_line() -> const char*
	{
#ifdef PLATFORM_WINDOWS
		return "\r\n";
#else
		return "\n";
#endif // PLATFORM_WINDOWS
	}

	bool environment::is_64bit_process()
	{
#ifndef TARGET_64BIT
		return false;
#else
		return true;
#endif // !TARGET_64BIT

	}
}
