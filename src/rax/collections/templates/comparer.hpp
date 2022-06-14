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
			return a == b;
		}

		template <typename S = equality_comparer> RAX_INLINE bool same_as() const
		{
			return false;

			//auto instance = S();
			//
			//return
			//	static_cast<uintptr_t>(&this->get_hashcode) == static_cast<uintptr_t>(&instance.get_hashcode) &&
			//	static_cast<uintptr_t>(&this->equals) == static_cast<uintptr_t>(&instance.equals);
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
