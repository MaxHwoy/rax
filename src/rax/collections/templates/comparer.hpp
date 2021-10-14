#pragma once

#include <cstdint>
#include <rax/hashcode.hpp>

namespace rax::collections::templates
{
	template <typename T> using value_equals = bool(*)(const T* a, const T* b);
	template <typename T> using value_hashcode = std::uint32_t(*)(const T* obj);

	template <typename T> class equality_comparer
	{
	private:
		value_equals<T> comparer_;
		value_hashcode<T> hash_getter_;
		std::uint64_t entropy_;
		bool is_randomized_;

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

			return rax::hashcode::compute(obj);
		}
		static auto get_entropy() -> std::uint64_t
		{
			// #TODO
		}

	public:
		equality_comparer() : comparer_(&default_equals), hash_getter_(&default_hashcode)
		{
		}
		equality_comparer(bool randomized) : comparer_(&default_equals), hash_getter_(&default_hashcode)
		{
			this->is_randomized_ = randomized;

			if (randomized)
			{
				this->entropy_ = equality_comparer::get_entropy();
			}
		}
		equality_comparer(value_equals<T> comparer, value_hashcode<T> hash_getter) : comparer_(comparer), hash_getter_(hash_getter)
		{
		}
		equality_comparer(value_equals<T> comparer, value_hashcode<T> hash_getter, bool randomized) : comparer_(comparer), hash_getter_(hash_getter)
		{
			this->is_randomized_ = randomized;

			if (randomized)
			{
				this->entropy_ = equality_comparer::get_entropy();
			}
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
			if (this->is_randomized_)
			{
				return hashcode::entropy(this->hash_getter_(obj), this->entropy_);
			}

			return this->hash_getter_(obj);
		}

		friend bool operator==(const equality_comparer& lhs, const equality_comparer& rhs);

		static bool is_well_known_comparer(const equality_comparer& value)
		{
			// not well known iff:
			//     - comparer or hash getter are not default ones
			//     - is not randomized

			if (value.comparer_ != &equality_comparer::default_equals ||
				value.hash_getter_ != &equality_comparer::default_hashcode)
			{
				return value.is_randomized_;
			}

			return true;
		}
	};

	template <typename T> bool operator==(const equality_comparer<T>& lhs, const equality_comparer<T>& rhs)
	{
		if (lhs.is_randomized_)
		{
			return rhs.is_randomized_ && lhs.entropy_ == rhs.entropy_;
		}

		if (rhs.is_randomized_)
		{
			return false;
		}

		return lhs.hash_getter_ == rhs.hash_getter_ && lhs.comparer_ == rhs.comparer_;
	}
}
