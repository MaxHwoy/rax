#pragma once

#include <cstdint>
#include <rax/text/strex.hpp>

namespace rax::collections::templates
{
	template <typename T> using value_equals = bool(*)(const T* a, const T* b);
	template <typename T> using value_hashcode = std::uint32_t(*)(const T* obj);

	template <typename T> class equality_comparer
	{
	private:
		value_equals<T> comparer_;
		value_hashcode<T> hash_getter_;

	private:
		static bool default_equals(const T* a, const T* b)
		{
			if (a == null)
			{
				return b == null;
			}
			
			if (b == null)
			{
				return false;
			}

			return *a == *b;
		}
		static auto default_hashcode(const T* obj) -> std::uint32_t
		{
			if (obj == null)
			{
				return 0u;
			}

			return rax::text::strex::mem_hashcode<T>(obj);
		}

	public:
		equality_comparer() : comparer_(&default_equals), hash_getter_(&default_hashcode)
		{
		}
		equality_comparer(value_equals<T> comparer, value_hashcode<T> hash_getter) : comparer_(comparer), hash_getter_(hash_getter)
		{
		}
		equality_comparer(const equality_comparer& other)
		{
			this->comparer_ = other.comparer_;
			this->hash_getter_ = other.hash_getter_;
		}
		equality_comparer(equality_comparer&& other)
		{
			this->comparer_ = other.comparer_;
			this->hash_getter_ = other.hash_getter_;
		}
		equality_comparer& operator=(const equality_comparer& other)
		{
			if (this != &other)
			{
				this->comparer_ = other.comparer_;
				this->hash_getter_ = other.hash_getter_;
			}

			return *this;
		}

		bool equals(const T* a, const T* b) const
		{
			return this->comparer_(a, b);
		}
		auto get_hashcode(const T* obj) const
		{
			return this->hash_getter_(obj);
		}
	};
}
