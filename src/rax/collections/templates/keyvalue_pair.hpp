#pragma once

#include <cstdint>
#include <rax/shared.hpp>

namespace rax::collections::templates
{
	template <typename TKey, typename TValue> class keyvalue_pair final
	{
	private:
		const TKey& key_;
		const TValue& value_;

	public:
		keyvalue_pair()
		{
		}
		keyvalue_pair(const TKey& key, const TValue& value) : key_(key), value_(value)
		{
		}

		auto key() const -> const TKey&
		{
			return this->key_;
		}
		auto value() const -> const TValue&
		{
			return this->value_;
		}
	};
}
