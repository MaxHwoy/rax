#include <rax/hashcode.hpp>

#pragma warning (disable : 4100) // unreferenced formal parameter

namespace rax
{
	void hashcode::internal_marvin32_mix(std::uint32_t* st, std::uint32_t num)
	{
		st[0] += num;
		st[1] ^= st[0];
		st[0] = bitwise::rotate_left(st[0], 0x14u) + st[1];
		st[1] = bitwise::rotate_left(st[1], 0x09u) ^ st[0];
		st[0] = bitwise::rotate_left(st[0], 0x1Bu) + st[1];
		st[1] = bitwise::rotate_left(st[1], 0x13u);
	}

	auto hashcode::internal_queue_rounds(std::uint32_t hash, std::uint32_t value) -> std::uint32_t
	{
		return bitwise::rotate_left(hash + value * 0xC2B2AE3Du, 0x11u) * 0x27D4EB2Fu;
	}

	auto hashcode::internal_combined_mix(std::uint32_t hash) -> std::uint32_t
	{
		hash ^= hash >> 15;
		hash *= 0x85EBCA77;
		hash ^= hash >> 13;
		hash *= 0xC2B2AE3D;
		hash ^= hash >> 16;

		return hash;
	}

	auto hashcode::compute(const void* ptr, std::uint32_t size) -> std::uint32_t
	{
		auto i = 0u;
		auto result = 0u;
		auto data = reinterpret_cast<const std::uint8_t*>(ptr);

		while (i != size)
		{
			result += data[i++];
			result += result << 10;
			result ^= result >> 6;
		}

		result += result << 3;
		result ^= result >> 11;
		result += result << 15;

		return result;
	}

	auto hashcode::compute(const void* ptr, std::uint32_t size, std::uint64_t seed) -> std::uint32_t
	{
		std::uint32_t st[2]
		{
			(std::uint32_t)(seed),
			(std::uint32_t)(seed >> 32),
		};

		auto data = reinterpret_cast<const std::uint8_t*>(ptr);

		while (size >= 4)
		{
			auto number = *reinterpret_cast<const std::uint32_t*>(data);

			hashcode::internal_marvin32_mix(st, number);
			data += 4;
			size -= 4;
		}

		std::uint32_t final = 0x80u;

		switch (size)
		{
			case 3:
				final = (final << 8) | data[2];
				[[fallthrough]];
				
			case 2:
				final = (final << 8) | data[1];
				[[fallthrough]];

			case 1:
				final = (final << 8) | data[0];
				[[fallthrough]];

			case 0:
			default:
				break;
		}

		hashcode::internal_marvin32_mix(st, final);
		hashcode::internal_marvin32_mix(st, 0u);

		return st[0] ^ st[1];
	}
	
	auto hashcode::combine(std::uint32_t key1, std::uint32_t key2) -> std::uint32_t
	{
		auto result = 0x165667B1u;

		result = hashcode::internal_queue_rounds(result, key1);
		result = hashcode::internal_queue_rounds(result, key2);

		return hashcode::internal_combined_mix(result);
	}

	auto hashcode::entropy(std::uint32_t hash, std::uint64_t entr) -> std::uint32_t
	{
		return 0u; // #TODO
	}

	//
	// inbuilt type hashcode computation
	//

	template <> static auto hashcode::compute(const char& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::int8_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::uint8_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::int16_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::uint16_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::int32_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj);
	}

	template <> static auto hashcode::compute(const std::uint32_t& obj) -> std::uint32_t
	{
		return obj;
	}

	template <> static auto hashcode::compute(const std::int64_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj) ^ static_cast<std::uint32_t>(obj >> 32);
	}

	template <> static auto hashcode::compute(const std::uint64_t& obj) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj) ^ static_cast<std::uint32_t>(obj >> 32);
	}

	template <> static auto hashcode::compute(const float& obj) -> std::uint32_t
	{
		if (obj == 0.0f)
		{
			return 0u;
		}

		return *reinterpret_cast<const std::uint32_t*>(&obj);
	}

	template <> static auto hashcode::compute(const double& obj) -> std::uint32_t
	{
		if (obj == 0.0)
		{
			return 0u;
		}

		auto value = *reinterpret_cast<const std::uint64_t*>(&obj);

		return static_cast<std::uint32_t>(value) ^ static_cast<std::uint32_t>(value >> 32);
	}

	template <> static auto hashcode::compute(const string& obj) -> std::uint32_t
	{
		if (obj.length() == 0u)
		{
			return 0u;
		}

		return hashcode::compute(reinterpret_cast<const void*>(obj.as_native()), obj.length());
	}

	template <> static auto hashcode::compute(const char& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::int8_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::uint8_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::int16_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::uint16_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::int32_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj); // ignore seed
	}

	template <> static auto hashcode::compute(const std::uint32_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return obj; // ignore seed
	}

	template <> static auto hashcode::compute(const std::int64_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj) ^ static_cast<std::uint32_t>(obj >> 32); // ignore seed
	}

	template <> static auto hashcode::compute(const std::uint64_t& obj, std::uint64_t seed) -> std::uint32_t
	{
		return static_cast<std::uint32_t>(obj) ^ static_cast<std::uint32_t>(obj >> 32); // ignore seed
	}

	template <> static auto hashcode::compute(const float& obj, std::uint64_t seed) -> std::uint32_t
	{
		if (obj == 0.0f)
		{
			return 0u;
		}

		return *reinterpret_cast<const std::uint32_t*>(&obj); // ignore seed
	}

	template <> static auto hashcode::compute(const double& obj, std::uint64_t seed) -> std::uint32_t
	{
		if (obj == 0.0)
		{
			return 0u;
		}

		auto value = *reinterpret_cast<const std::uint64_t*>(&obj);

		return static_cast<std::uint32_t>(value) ^ static_cast<std::uint32_t>(value >> 32); // ignore seed
	}

	template <> static auto hashcode::compute(const string& obj, std::uint64_t seed) -> std::uint32_t
	{
		if (obj.length() == 0u)
		{
			return 0u;
		}

		return hashcode::compute(reinterpret_cast<const void*>(obj.as_native()), obj.length(), seed);
	}
}
