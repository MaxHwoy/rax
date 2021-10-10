#include <rax/time/time.hpp>

namespace rax
{
	time::point_type time::start_time_ = rax::time::now();

	auto time::now() -> time::point_type
	{
		return std::chrono::system_clock::now();
	}

	auto time::since(const time::point_type& point) -> time::result_type
	{
		return time::now() - point;
	}

	auto time::since_start() -> time::result_type
	{
		return time::since(time::start_time_);
	}

	auto time::to_nanoseconds(const time::result_type& value) -> std::chrono::nanoseconds::rep
	{
		return time::to<std::chrono::nanoseconds>(value);
	}

	auto time::to_microseconds(const time::result_type& value) -> std::chrono::microseconds::rep
	{
		return time::to<std::chrono::microseconds>(value);
	}

	auto time::to_milliseconds(const time::result_type& value) -> std::chrono::milliseconds::rep
	{
		return time::to<std::chrono::milliseconds>(value);
	}

	auto time::to_seconds(const time::result_type& value) -> std::chrono::milliseconds::rep
	{
		return time::to<std::chrono::seconds>(value);
	}

	auto time::to_minutes(const time::result_type& value) -> std::chrono::milliseconds::rep
	{
		return time::to<std::chrono::minutes>(value);
	}

	auto time::to_hours(const time::result_type& value) -> std::chrono::milliseconds::rep
	{
		return time::to<std::chrono::hours>(value);
	}
}
