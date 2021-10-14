#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <initializer_list>
#include <rax/collections/hash_helpers.hpp>
#include <rax/collections/templates/keyvalue_pair.hpp>
#include <rax/collections/templates/comparer.hpp>

namespace rax::collections::templates
{
	template <typename TKey, typename TValue> class map final
	{
	private:
		struct entry
		{
			std::uint32_t hashcode;
			std::int32_t next;
			TKey key;
			TValue value;

			entry()
			{
			}
			entry(const entry& other)
			{
				this->hashcode = other.hashcode;
				this->next = other.next;
				this->key = std::move(other.key);
				this->value = std::move(other.value);
			}
			entry(entry&& other) noexcept
			{
				this->hashcode = other.hashcode;
				this->next = other.next;
				this->key = std::move(other.key);
				this->value = std::move(other.value);
			}
			entry& operator=(const entry& other)
			{
				if (this != &other)
				{
					this->hashcode = other.hashcode;
					this->next = other.next;
					this->key = std::move(other.key);
					this->value = std::move(other.value);
				}

				return *this;
			}
		};

		std::int32_t* buckets_;
		entry* entries_;
		std::uint32_t capacity_;
		std::uint32_t count_;
		std::int32_t free_list_;
		std::uint32_t free_count_;
		equality_comparer<TKey> comparer_;

	private:
		void initialize(std::uint32_t capacity)
		{
			this->capacity_ = hash_helpers::get_prime(capacity);

			this->buckets_ = new std::int32_t[this->capacity_];
			this->entries_ = new entry[this->capacity_];

			this->count_ = 0u;
			this->free_list_ = -1;
			this->free_count_ = 0u;

			::memset(this->buckets_, -1, this->capacity_ * sizeof(std::int32_t));
		}
		auto find_entry(const TKey& key) -> std::int32_t
		{
			auto hash = this->comparer_.get_hashcode(&key) & INT32_MAX;

			for (std::int32_t i = this->buckets_[hash % this->capacity_]; i >= 0; i = this->entries_[i].next)
			{
				if (this->entries_[i].hashcode == hash && this->comparer_.equals(&this->entries_[i].key, &key))
				{
					return i;
				}
			}

			return -1;
		}
		bool insert(const TKey& key, const TValue& value, bool force)
		{
			auto hashcode = this->comparer_.get_hashcode(&key) & INT32_MAX;
			auto position = hashcode % this->capacity_;
			auto number = 0u;

			for (std::int32_t i = this->buckets_[position]; i >= 0; i = this->entries_[i].next)
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

			std::int32_t available = 0;

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
				this->comparer_ = equality_comparer<TKey>(true);
				this->resize(this->capacity_, true);
			}

			return true;
		}
		bool moveto(TKey&& key, TValue&& value, bool force)
		{
			auto hashcode = this->comparer_.get_hashcode(&key) & INT32_MAX;
			auto position = hashcode % this->capacity_;
			auto number = 0u;

			for (std::int32_t i = this->buckets_[position]; i >= 0; i = this->entries_[i].next)
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

			std::int32_t available = 0;

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
				this->comparer_ = equality_comparer<TKey>(true);
				this->resize(this->capacity_, true);
			}

			return true;
		}
		void resize()
		{
			this->resize(hash_helpers::expand_prime(this->count_), false);
		}
		void resize(std::uint32_t newSize, bool forceNewHashes)
		{
			auto buckets = new std::int32_t[newSize];
			auto entries = new entry[newSize];

			::memset(buckets, -1, newSize * sizeof(std::int32_t));

			for (std::uint32_t i = 0u; i < this->count_; ++i)
			{
				entries[i] = this->entries_[i];
			}

			if (forceNewHashes)
			{
				for (std::uint32_t i = 0u; i < this->count_; ++i)
				{
					if (entries[i].hashcode != UINT32_MAX)
					{
						entries[i].hashcode = this->comparer_.get_hashcode(&entries[i].key) & INT32_MAX;
					}
				}
			}

			for (std::uint32_t i = 0u; i < this->count_; ++i)
			{
				if (entries[i].hashcode != -1)
				{
					auto num = entries[i].hashcode % newSize;
					entries[i].next = buckets[num];
					buckets[num] = static_cast<std::int32_t>(i);
				}
			}

			delete[] this->buckets_;
			delete[] this->entries_;

			this->buckets_ = buckets;
			this->entries_ = entries;
			this->capacity_ = newSize;
		}

	public:
		map() : map(0u, equality_comparer<TKey>::get_default())
		{
		}
		map(std::uint32_t capacity) : map(capacity, equality_comparer<TKey>::get_default())
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
		map(std::initializer_list<keyvalue_pair<TKey, TValue>> init_list)
		{
			auto capacity = init_list.size() < 7u ? 7u : init_list.size();

			this->initialize(capacity);

			this->comparer_ = equality_comparer<TKey>::get_default();

			for (auto it = init_list.begin(); it != init_list.end(); ++it)
			{
				this->add(it->key(), it->value());
			}
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

		auto count() const -> std::uint32_t
		{
			return this->count_;
		}
		auto comparer() const -> const equality_comparer<TKey>&
		{
			return this->comparer_;
		}

		auto operator[](const TKey& key) -> TValue&
		{
			return TValue{};
		}
		bool add(const TKey& key, const TValue& value)
		{
			return this->insert(key, value, false);
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
		bool put(TKey&& key, TValue&& value)
		{
			return this->moveto(std::forward<TKey>(key), std::forward<TValue>(value), false);
		}
		bool remove(const TKey& key)
		{
			return false;
		}
		bool try_get_value(const TKey& key, TValue& value)
		{
			auto result = this->find_entry(key);

			if (result >= 0)
			{
				value = this->entries_[result].value;
				return true;
			}

			return false;
		}
		void update(const TKey& key, const TValue& value)
		{
			this->insert(key, value, true);
		}
	};
}
