#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax
{
	class environment final
	{
	private:
		static inline wchar_t internal_buffer_[0x1000]; // max environment variable size

	public:
		static auto processor_count() -> std::uint32_t;
		static bool is_single_processor();

		static auto get_environment_variable(const wchar_t* variable) -> const wchar_t*;
		static void set_environment_variable(const char* variable, const char* value);
		static void set_environment_variable(const wchar_t* variable, const wchar_t* value);

		static auto process_id() -> std::uint32_t;
		static auto new_line() -> const char*;

		static bool is_64bit_process();
	};
}
