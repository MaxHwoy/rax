#include <rax/environment.hpp>

#include <thread>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif // PLATFORM_WINDOWS

#ifdef PLATFORM_UNIX
#include <unistd.h>
#endif

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

	}




	auto environment::process_id() -> std::uint32_t
	{
#ifdef PLATFORM_WINDOWS
		return ::GetCurrentProcessId();
#else
		return ::getpid();
#endif
	}




	bool environment::is_64bit_process()
	{
#ifndef TARGET_64BIT
		return false;
#else
		return true;
#endif // !_M_X64

	}
}
