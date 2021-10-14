#pragma once

#include <cstdint>
#include <memory>
#include <rax/collections/hash_helpers.hpp>
#include <rax/collections/templates/comparer.hpp>

namespace rax::collections::templates
{
	template <typename TKey, typename TValue> class map final
	{
	private:
		struct entry
		{
			std::uint32_t hashcode;
			std::uint32_t next;
			TKey key;
			TValue value;
		};

		std::uint32_t* buckets_;
		entry* entries_;
		std::uint32_t capacity_;
		std::uint32_t count_;
		std::uint32_t free_list_;
		std::uint32_t free_count_;
		equality_comparer<TKey> comparer_;

	private:
		void initialize(std::uint32_t capacity)
		{
			this->capacity_ = hash_helpers::get_prime(capacity);

			this->buckets_ = new std::uint32_t[this->capacity_];
			this->entries_ = new entry[this->capacity_];

			this->count_ = 0u;
			this->free_list_ = 0u;
			this->free_count_ = 0u;

			::memset(this->buckets_, -1, this->capacity_ * sizeof(std::uint32_t));
		}
		void resize()
		{
			this->resize(hash_helpers::expand_prime(this->count_), false);
		}
		void resize(std::uint32_t newSize, bool forceNewHashes)
		{

		}
		bool insert(const TKey& key, const TValue& value, bool force)
		{
			if (this->buckets_ == null)
			{
				this->initialize(7u);
			}

			auto hashcode = this->comparer_.get_hashcode(&key);
			auto position = hashcode % this->capacity_;
			auto number = 0u;

			for (std::uint32_t i = this->buckets_[position]; i != UINT32_MAX; i = this->entries_[i].next)
			{
				if (this->entries_[i].hashcode == hashcode &&
					this->comparer_.equals(&this->entries_[i].key, &key))
				{
					if (!force)
					{
						return false;
					}
					else
					{
						this->entries_[i].value = value;
						return true;
					}
				}

				++number;
			}

			auto available = 0u;

			if (this->free_count_ > 0)
			{
				available = this->free_list_;
				this->free_list_ = this->entries_[available].next;
				--this->free_count_;
			}
			else
			{
				if (this->count_ == this->capacity_)
				{
					this->resize();
					position = hashcode % this->capacity_;
				}

				available = this->count_;
				++this->count_;
			}

			auto& newentry = this->entries_[available];

			newentry.hashcode = hashcode;
			newentry.next = this->buckets_[position];
			newentry.key = key;
			newentry.value = value;

			this->buckets_[position] = available;

			if (number > 100 && equality_comparer<TKey>::is_well_known_comparer(this->comparer_))
			{
				// #TODO: get new randomized equality comparer
				this->resize(this->capacity_, true);
			}
		}

	public:
		map() : map(0u, equality_comparer<TKey>())
		{
		}
		map(std::uint32_t capacity) : map(capacity, equality_comparer<TKey>())
		{
		}
		map(const equality_comparer<TKey>& comparer) : map(0u, comparer)
		{
		}
		map(std::uint32_t capacity, const equality_comparer<TKey>& comparer)
		{
			capacity = capacity < 7u ? 7u : capacity;
			
			this->comparer_ = comparer;

			this->initialize(capacity);
		}

		// initializer list too
		// we need generic enumerators too
		
		map(const map& other);
		map(map&& other);
		map& operator=(const map& other);
		~map()
		{
			// cleanup
		}


		auto operator[](const TKey& key) -> TValue&
		{
			return TValue{};
		}
		bool add(const TKey& key, const TValue& value)
		{
			return this->insert(key, value);
		}
		void clear()
		{

		}
		bool contains_key(const TKey& key) const
		{
			return false;
		}
		bool contains_value(const TValue& value) const
		{
			return false;
		}
		bool remove(const TKey& key)
		{
			return false;
		}
		bool try_get_value(const TKey& key, TValue& value)
		{
			return false;
		}
	};
}
