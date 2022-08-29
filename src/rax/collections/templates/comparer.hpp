#pragma once

#include <cstdint>
#include <rax/string.hpp>
#include <rax/hashcode.hpp>
#include <rax/collections/hash_helpers.hpp>

namespace rax::collections::templates
{
	template <typename T> class equality_comparer
	{
	public:
		auto get_hashcode(const T& obj) const -> std::uint32_t
		{
			return hashcode::compute(obj);
		}

		bool equals(const T& a, const T& b) const
		{
			const auto lhs = reinterpret_cast<const T*>(&a);
			const auto rhs = reinterpret_cast<const T*>(&b);

			return ::memcmp(lhs, rhs, sizeof(T)) == 0;
		}
	};

	class non_randomized_string_comparer
	{
	public:
		class ordinal_comparer
		{
		public:
			auto get_hashcode(const string& obj) const -> std::uint32_t
			{
				return 0u; // #TODO
			}

			bool equals(const string& a, const string& b) const
			{
				return string::equals(a, b, string_comparison::ordinal);
			}
		};

		class ordinal_ignore_case_comparer
		{
		public:
			auto get_hashcode(const string& obj) const -> std::uint32_t
			{
				return 0u; // #TODO
			}

			bool equals(const string& a, const string& b) const
			{
				return string::equals(a, b, string_comparison::ordinal_ignore_case);
			}
		};
	};

	class randomized_string_comparer
	{
	public:
		class ordinal_comparer
		{
		private:
			std::uint64_t m_seed;

		public:
			RAX_INLINE ordinal_comparer()
			{
				this->m_seed = hashcode::get_entropy();
			}

			auto get_hashcode(const string& obj) const -> std::uint32_t
			{
				return hashcode::compute(obj, this->m_seed);
			}

			bool equals(const string& a, const string& b) const
			{
				return string::equals(a, b, string_comparison::ordinal);
			}
		};

		class ordinal_ignore_case_comparer
		{
		private:
			std::uint64_t m_seed;

		public:
			RAX_INLINE ordinal_ignore_case_comparer()
			{
				this->m_seed = hashcode::get_entropy();
			}

			auto get_hashcode(const string& obj) const -> std::uint32_t
			{
				return hashcode::compute(obj, this->m_seed); // #TODO
			}

			bool equals(const string& a, const string& b) const
			{
				return string::equals(a, b, string_comparison::ordinal_ignore_case);
			}
		};
	};
}
