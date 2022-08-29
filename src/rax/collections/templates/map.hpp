#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <initializer_list>
#include <rax/objective.hpp>
#include <rax/hashcode.hpp>
#include <rax/collections/hash_helpers.hpp>
#include <rax/collections/templates/keyvalue_pair.hpp>
#include <rax/collections/templates/comparer.hpp>

namespace rax::collections::templates
{
	template <
		typename tkey,
		typename tvalue,
		typename comparer = equality_comparer<tkey>,
		typename alloc = rax::allocator
	> class map
	{
	private:
		enum insertion_behavior : std::uint8_t
		{
			none = 0,
			overwrite_existing = 1,
			throw_on_existing = 2
		};

		struct entry
		{
			std::uint32_t hash_code;
			std::int32_t next;
			tkey key;
			tvalue value;
		};

	public:
		class key_collection
		{

		};

		class value_collection
		{

		};

	private:
		entry* m_entries;
		std::int32_t* m_buckets;
		std::uint32_t m_buckets_len;
		std::uint32_t m_entries_len;
		std::uint64_t m_fastModMultiplier;
		std::int32_t m_count;
		std::int32_t m_freeList;
		std::int32_t m_freeCount;
		static inline const std::int32_t k_start_of_free_list = -3;

		comparer m_comparer;

	public:
		map() : map(0u)
		{
		}
		map(std::uint32_t capacity) :
			m_entries(nullptr),
			m_buckets(nullptr),
			m_buckets_len(0u),
			m_entries_len(0u),
			m_fastModMultiplier(0ui64),
			m_count(0),
			m_freeList(0),
			m_freeCount(0),
			m_comparer(comparer())
		{
			if (capacity < 0)
			{
				capacity = 0;
			}

			if (capacity > 0)
			{
				this->initialize(capacity);
			}
		}

	private:
		void add_range(const map<tkey, tvalue>& mapper)
		{
			if (mapper.count() == 0)
			{
				// Nothing to copy, all done
				return;
			}

			// This is not currently a true .add_range as it needs to be an initialized map
			// of the correct size, and also an empty map with no current entities (and no argument checks).
			assert(mapper.m_entries != nullptr);
			assert(this->m_entries != nullptr);
			assert(this->m_entries_len >= mapper.count());
			assert(this->m_count == 0);

			auto old_entries = map.m_entries;

			// #TODO
		}

		RAX_INLINE auto get_bucket(std::uint32_t hash_code) const -> std::int32_t*
		{
#ifdef TARGET_64BIT
			return this->m_buckets + hash_helpers::fast_mod(hash_code, this->m_buckets_len, this->m_fastModMultiplier);
#else
			return this->m_buckets + (hash_code % this->m_buckets_len);
#endif
		}

		void initialize(std::uint32_t capacity)
		{
			auto size = hash_helpers::get_prime(capacity);
			auto buckets = reinterpret_cast<std::int32_t*>(alloc().allocate(sizeof(std::int32_t), size));
			auto entries = reinterpret_cast<entry*>(alloc().allocate(sizeof(entry), size));

			this->m_freeList = -1;
			this->m_buckets = buckets;
			this->m_entries = entries;
			this->m_buckets_len = size;
			this->m_entries_len = size;
			this->m_fastModMultiplier = hash_helpers::get_fast_mod_multiplier(static_cast<std::uint32_t>(size));
		}

		auto find_value(tkey key) const -> tvalue*
		{
			entry* entry_ptr = nullptr;

			if (this->m_buckets != nullptr)
			{
				assert(this->m_entries != nullptr);

				auto hash_code = this->m_comparer.get_hashcode(key);
				auto i = *this->get_bucket(hash_code);
				auto collision_count = 0u;

				i--;

				do
				{
					if (static_cast<std::uint32_t>(i) >= this->m_entries_len)
					{
						return nullptr;
					}

					entry_ptr = this->m_entries + i;

					if (entry_ptr->hash_code == hash_code && this->m_comparer.equals(entry_ptr->key, key))
					{
						return &entry_ptr->value;
					}

					i = entry_ptr->next;
					collision_count++;

				} while (collision_count <= static_cast<std::uint32_t>(this->m_entries_len));

				throw new std::exception("Concurrent operations are not supported");
			}

			return nullptr;
		}
		auto find_value(const tkey* key) const -> tvalue*
		{
			if (key == nullptr)
			{
				return nullptr;
			}

			entry* entry_ptr = nullptr;

			if (this->m_buckets != nullptr)
			{
				assert(this->m_entries != nullptr);

				auto hash_code = this->m_comparer.get_hashcode(key);
				auto i = *this->get_bucket(hash_code);
				auto collision_count = 0u;

				i--;

				do
				{
					if (static_cast<std::uint32_t>(i) >= static_cast<std::uint32_t>(this->m_entries_len))
					{
						return nullptr;
					}

					entry_ptr = this->m_entries + i;

					if (entry_ptr->hash_code == hash_code && this->m_comparer.equals(entry_ptr->key, *key))
					{
						return &entry_ptr->value;
					}

					i = entry_ptr->next;
					collision_count++;

				} while (collision_count <= static_cast<std::uint32_t>(this->m_entries_len));

				throw new std::exception("Concurrent operations are not supported");
			}

			return nullptr;
		}

		auto get_value_ptr_or_add_default(tkey key) -> tvalue&
		{
			if (this->m_buckets == nullptr)
			{
				this->initialize(0u);
			}

			assert(this->m_buckets != nullptr);
			assert(this->m_entries != nullptr);

			auto entries = this->m_entries;

			auto hash_code = this->m_comparer.get_hashcode(key);
			auto bucket = this->get_bucket(hash_code);
			auto collision_count = 0u;
			auto i = *bucket - 1;

			while (true)
			{
				if (static_cast<std::uint32_t>(i) >= static_cast<std::uint32_t>(this->m_entries_len))
				{
					break;
				}

				if (entries[i].hash_code == hash_code && this->m_comparer.equals(entries[i].key, key))
				{
					return entries[i].value;
				}

				i = entries[i].next;
				collision_count++;

				if (collision_count > static_cast<std::uint32_t>(this->m_entries_len))
				{
					throw new std::exception("Concurrent operations are not supported");
				}
			}

			int index;

			if (this->m_freeCount > 0)
			{
				index = this->m_freeList;
				this->m_freeList = k_start_of_free_list - entries[this->m_freeList].next;
				this->m_freeCount--;
			}
			else
			{
				int count = this->m_count;

				if (count == this->m_entries_len)
				{
					this->resize();
					bucket = this->get_bucket(hash_code);
				}

				index = count;
				this->m_count = count + 1;
				entries = this->m_entries;
			}

			auto entry_ptr = entries + index;

			entry_ptr->hash_code = hash_code;
			entry_ptr->next = *bucket - 1;
			entry_ptr->key = key;

			*bucket = index + 1;

			// blittable types never get rehashed

			if constexpr (!is_blittable<tkey>::value /* && #TODO string types */)
			{
				if (collision_count > 100)
				{
					this->resize(this->m_entries_len, true);

					return *this->find_value(key);
				}
			}

			return entry_ptr->value;
		}
		auto get_value_ptr_or_add_default(const tkey* key) -> tvalue&
		{
			if (key == nullptr)
			{
				throw std::exception("Key pointer cannot be a null pointer");
			}

			if (this->m_buckets == nullptr)
			{
				this->initialize(0u);
			}

			assert(this->m_buckets != nullptr);
			assert(this->m_entries != nullptr);

			auto entries = this->m_entries;

			auto hash_code = this->m_comparer.get_hashcode(key);
			auto bucket = this->get_bucket(hash_code);
			auto collision_count = 0u;
			auto i = *bucket - 1;

			while (true)
			{
				if (static_cast<std::uint32_t>(i) >= static_cast<std::uint32_t>(this->m_entries_len))
				{
					break;
				}

				if (entries[i].hash_code == hash_code && this->m_comparer.equals(entries[i].key, *key))
				{
					return entries[i].value;
				}

				i = entries[i].next;
				collision_count++;

				if (collision_count > static_cast<std::uint32_t>(this->m_entries_len))
				{
					throw new std::exception("Concurrent operations are not supported");
				}
			}

			int index;

			if (this->m_freeCount > 0)
			{
				index = this->m_freeList;
				this->m_freeList = this->kStartOfFreeList - entries[this->m_freeList].next;
				this->m_freeCount--;
			}
			else
			{
				int count = this->m_count;

				if (count == this->m_entries_len)
				{
					this->resize();
					bucket = this->get_bucket(hash_code);
				}

				index = count;
				this->m_count = count + 1;
				entries = this->m_entries;
			}

			auto entry_ptr = entries + index;

			entry_ptr->hash_code = hash_code;
			entry_ptr->next = *bucket - 1;
			entry_ptr->key = *key;

			*bucket = index + 1;

			if (this->m_comparer.same_as<non_randomized_string_comparer::ordinal_comparer>() ||
				this->m_comparer.same_as<non_randomized_string_comparer::ordinal_ignore_case_comparer>())
			{
				if (collision_count > 100)
				{
					this->resize(this->m_entries_len, true);

					return *this->find_value(key);
				}
			}

			return entry_ptr->value;
		}

		void resize()
		{
			this->resize(hash_helpers::expand_prime(this->m_count), false);
		}
		void resize(int new_size, bool force_new_hash_codes)
		{

		}

	public:
		auto operator[](tkey key) -> tvalue&
		{
			return this->get_value_ptr_or_add_default(key);
		}
		auto operator[](tkey* key) -> tvalue&
		{
			return this->get_value_ptr_or_add_default(key);
		}

		auto count() const -> std::int32_t
		{
			return this->m_count - this->m_freeCount;
		}
		auto keys() const->key_collection;
		auto values() const->value_collection;

		bool contains_key(tkey key) const
		{
			return this->find_value(key) != nullptr;
		}
		bool contains_key(const tkey* key) const
		{
			return this->find_value(key) != nullptr;
		}

		bool contains_value(tvalue value) const
		{
			for (std::int32_t i = 0; i < this.m_count; ++i)
			{
				if (this->m_entries[i].next >= -1 && value == this->m_entries[i].value)
				{
					return true;
				}
			}

			return false;
		}
		bool contains_value(const tvalue* value) const
		{
			for (std::int32_t i = 0; i < this.m_count; ++i)
			{
				if (this->m_entries[i].next >= -1 && *value == this->m_entries[i].value)
				{
					return true;
				}
			}

			return false;
		}

		bool try_get_value(tkey key, tvalue*& value) const
		{
			value = this->find_value(key);

			return value != nullptr;
		}
		bool try_get_value(const tkey* key, tvalue*& value) const
		{
			value = this->find_value(key);

			return value != nullptr;
		}
	};
}
