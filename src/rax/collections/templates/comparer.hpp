#pragma once

#include <cstdint>
#include <rax/hashcode.hpp>
#include <rax/collections/hash_helpers.hpp>

namespace rax::collections::templates
{
	template <typename T> using value_equals = bool(*)(const T* a, const T* b);
	template <typename T> using value_hashcode = std::uint32_t(*)(const T* obj);
	template <typename T> using value_hash_random = std::uint32_t(*)(const T* obj, std::uint64_t entropy);

	template <typename T> class equality_comparer
	{
	private:
		static equality_comparer default_;

	private:
		value_equals<T> comparer_;
		value_hashcode<T> hash_getter_;
		value_hash_random<T> hash_random_;
		std::uint64_t entropy_;

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

			return rax::hashcode::compute(*obj);
		}
		static auto default_hash_random(const T* obj, std::uint64_t entropy) -> std::uint32_t
		{
			if (obj == null)
			{
				return 0u;
			}

			return rax::hashcode::compute(*obj, entropy);
		}

	public:
		equality_comparer() : comparer_(&default_equals), hash_getter_(&default_hashcode), hash_random_(null), entropy_(0u)
		{
		}
		equality_comparer(bool random) : comparer_(&default_equals)
		{
			if (random)
			{
				this->hash_getter_ = null;
				this->hash_random_ = &default_hash_random;
				this->entropy_ = hash_helpers::get_entropy();
			}
			else
			{
				this->hash_getter_ = &default_hashcode;
				this->hash_random_ = null;
				this->entropy_ = 0u;
			}
		}
		equality_comparer(value_equals<T> comparer, value_hashcode<T> hash_getter)
			: comparer_(comparer), hash_getter_(hash_getter), hash_random_(null), entropy_(0u)
		{
		}
		equality_comparer(value_equals<T> comparer, value_hash_random<T> hash_random)
			: comparer_(comparer), hash_getter_(null), hash_random_(hash_random)
		{
			if (hash_random == null)
			{
				this->hash_getter_ = &equality_comparer::default_hash_random;
			}

			this->entropy_ = hash_helpers::get_entropy();
		}
		equality_comparer(const equality_comparer& other)
		{
			this->comparer_ = other.comparer_;
			this->hash_getter_ = other.hash_getter_;
			this->hash_random_ = other.hash_random_;
			this->entropy_ = other.entropy_;
		}
		equality_comparer(equality_comparer&& other) noexcept
		{
			this->comparer_ = other.comparer_;
			this->hash_getter_ = other.hash_getter_;
			this->hash_random_ = other.hash_random_;
			this->entropy_ = other.entropy_;
		}
		equality_comparer& operator=(const equality_comparer& other)
		{
			if (this != &other)
			{
				this->comparer_ = other.comparer_;
				this->hash_getter_ = other.hash_getter_;
				this->hash_random_ = other.hash_random_;
				this->entropy_ = other.entropy_;
			}

			return *this;
		}

		bool equals(const T* a, const T* b) const
		{
			return this->comparer_(a, b);
		}
		auto get_hashcode(const T* obj) const
		{
			if (this->entropy_ > 0u)
			{
				return this->hash_random_(obj, this->entropy_);
			}

			return this->hash_getter_(obj);
		}

		friend bool operator==(const equality_comparer& lhs, const equality_comparer& rhs)
		{
			// Comparison type:
			//     - if entropy is 0, then hash_random_ is null, so we check only comparer_ and hash_getter_
			//     - if entropy is not 0, then it is unique, so the only way two instances are equal is when they have same entropy

			if (lhs.entropy_ > 0)
			{
				return lhs.entropy_ == rhs.entropy_;
			}

			if (rhs.entropy_ > 0)
			{
				return false;
			}

			return lhs.hash_getter_ == rhs.hash_getter_ && lhs.comparer_ == rhs.comparer_;
		}

		static auto get_default() -> const equality_comparer&
		{
			return equality_comparer::default_;
		}
		static bool is_well_known_comparer(const equality_comparer& value)
		{
			// not well known iff:
			//     - not equals default comparer
			//     - is not randomized

			if (value != equality_comparer::default_)
			{
				return value.entropy_ > 0u;
			}

			return true;
		}
	};

	template <typename T> equality_comparer<T> equality_comparer<T>::default_ = equality_comparer<T>();
}
