#pragma once

#include <cstdint>
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

		std::int32_t* buckets_;
		entry* entries_;
		std::uint32_t capacity_;
		std::uint32_t count_;
		equality_comparer<TKey> comparer_;

	private:
		void initialize(std::uint32_t capacity)
		{

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

		// we need generic enumerators too
		
		map(const map& other);
		map(map&& other);
		map& operator=(const map& other);
		~map()
		{

		}


		auto operator[](const TKey& key) -> TValue&;
		void add(const TKey& key, const TValue& value);
		void clear();
		bool contains_key(const TKey& key);
		bool contains_value(const TValue& value);
		bool remove(const TKey& key);
		bool try_get_value(const TKey& key, TValue& value);
	};
}
