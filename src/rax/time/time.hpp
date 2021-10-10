#pragma once

#include <cstdint>
#include <chrono>
#include <rax/shared.hpp>

namespace rax
{
	class time final
	{
	public:
		using point_type = std::chrono::system_clock::time_point;
		using result_type = std::chrono::system_clock::duration;

	private:
		static time::point_type start_time_;

		template <typename T> static RAX_INLINE auto to(const time::result_type& value) -> typename T::rep
		{
			return std::chrono::duration_cast<T>(value).count();
		}

	public:
		static auto now() -> time::point_type;
		static auto since(const time::point_type& point) -> time::result_type;
		static auto since_start() -> time::result_type;

		static auto to_nanoseconds(const time::result_type& value) -> std::chrono::nanoseconds::rep;
		static auto to_microseconds(const time::result_type& value) -> std::chrono::microseconds::rep;

		static auto to_milliseconds(const time::result_type& value) -> std::chrono::milliseconds::rep;
		static auto to_seconds(const time::result_type& value) -> std::chrono::milliseconds::rep;

		static auto to_minutes(const time::result_type& value) -> std::chrono::milliseconds::rep;
		static auto to_hours(const time::result_type& value) -> std::chrono::milliseconds::rep;
	};

	class timer final
	{
	private:
		time::point_type start_time_;

	public:
		RAX_INLINE timer()
		{
			this->reset();
		}
		RAX_INLINE auto elapsed() const -> time::result_type
		{
			return time::now() - this->start_time_;
		}
		RAX_INLINE void reset()
		{
			this->start_time_ = time::now();
		}
	};
}
