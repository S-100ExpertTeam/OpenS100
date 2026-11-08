#pragma once
// Small helpers for the record maps (std::unordered_map<int64_t, Record*>) that replaced
// MFC CMap in the dataset classes. Phase 1 step 3-3.
#include <cstdint>
#include <unordered_map>

namespace s100
{
	// CMap::Lookup equivalent: sets `out` and returns true when `key` is present.
	template <class Map, class Value>
	[[nodiscard]] bool Lookup(const Map& map, const typename Map::key_type& key, Value& out)
	{
		const auto it = map.find(key);
		if (it == map.end())
		{
			return false;
		}
		out = it->second;
		return true;
	}

	template <class Record>
	using RecordMap = std::unordered_map<std::int64_t, Record*>;
}
