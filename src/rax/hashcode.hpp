#pragma once

#include <cstdint>
#include <string>
#include <rax/shared.hpp>
#include <rax/bitwise.hpp>
#include <rax/string.hpp>

namespace rax
{
	class hashcode final
	{
	private:
		static void internal_marvin32_mix(std::uint32_t* st, std::uint32_t num);
		static auto internal_queue_rounds(std::uint32_t hash, std::uint32_t value) -> std::uint32_t;
		static auto internal_combined_mix(std::uint32_t hash) -> std::uint32_t;

	public:
		static auto compute_raw(const void* ptr, std::uint32_t size) -> std::uint32_t;
		static auto compute_raw(const void* ptr, std::uint32_t size, std::uint64_t seed) -> std::uint32_t;
		
		static auto combine(std::uint32_t key1, std::uint32_t key2) -> std::uint32_t;
		static auto entropy(std::uint32_t hash, std::uint64_t entr) -> std::uint32_t;

		static auto get_entropy() -> std::uint64_t;

		template <typename T> static auto compute(const T& obj) -> std::uint32_t
		{
			return hashcode::compute_raw(reinterpret_cast<const void*>(&obj), sizeof(T));
		}
		template <typename T> static auto compute(const T& obj, std::uint64_t seed) -> std::uint32_t
		{
			return hashcode::compute_raw(reinterpret_cast<const void*>(&obj), sizeof(T), seed);
		}

		template <typename T1, typename T2> static auto combine(const T1& value1, const T2& value2) -> std::uint32_t
		{
			auto hc1 = hashcode::compute(value1);
			auto hc2 = hashcode::compute(value2);

			return hashcode::combine(hc1, hc2);
		}
		template <typename T1, typename T2, typename T3> static auto combine(const T1& value1, const T2& value2, const T3& value3) -> std::uint32_t
		{
			auto hc1 = hashcode::compute(value1);
			auto hc2 = hashcode::compute(value2);
			auto hc3 = hashcode::compute(value3);

			return hashcode::combine(hashcode::combine(hc1, hc2), hc3);
		}
	};

	//
	// inbuilt type hashcode computation
	//

	template <> static auto hashcode::compute(const char& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int8_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint8_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int16_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint16_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int32_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint32_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int64_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint64_t& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const float& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const double& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const string& obj) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::string& obj) -> std::uint32_t;

	template <> static auto hashcode::compute(const char& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int8_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint8_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int16_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint16_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int32_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint32_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::int64_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::uint64_t& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const float& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const double& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const string& obj, std::uint64_t seed) -> std::uint32_t;
	template <> static auto hashcode::compute(const std::string& obj, std::uint64_t seed) -> std::uint32_t;
}
