#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>

// A least-recently-used cache with a capacity in cost units: one unit per entry to bound the
// number of entries, or bytes to bound memory. Used to keep datasets that left the selection
// open for a while, so a view that comes back does not parse them again
// (Documents/TechDocs/2026-11-10-system-database.md, SDB-6).
namespace s100::sdb
{
    template <typename Key, typename Value, typename Hash = std::hash<Key>>
    class LruCache final
    {
    public:
        explicit LruCache(std::size_t capacity) : capacity_(capacity) {}

        [[nodiscard]] std::size_t capacity() const noexcept { return capacity_; }
        [[nodiscard]] std::size_t cost() const noexcept { return cost_; }
        [[nodiscard]] std::size_t size() const noexcept { return index_.size(); }
        [[nodiscard]] bool empty() const noexcept { return index_.empty(); }
        [[nodiscard]] bool contains(const Key& key) const { return index_.contains(key); }

        // The value, now the most recently used; nullptr when absent.
        [[nodiscard]] Value* find(const Key& key)
        {
            const auto found = index_.find(key);
            if (found == index_.end())
            {
                return nullptr;
            }
            entries_.splice(entries_.begin(), entries_, found->second);
            return &found->second->value;
        }

        // Inserts or replaces the value as the most recently used, then evicts the least recently
        // used entries until the cost fits the capacity. An entry costing more than the whole
        // capacity is not kept.
        void put(Key key, Value value, std::size_t entryCost = 1)
        {
            (void)take(key);
            entries_.push_front(Entry{ key, std::move(value), entryCost });
            index_.emplace(std::move(key), entries_.begin());
            cost_ += entryCost;
            evict();
        }

        // Removes the value and hands it back.
        [[nodiscard]] std::optional<Value> take(const Key& key)
        {
            const auto found = index_.find(key);
            if (found == index_.end())
            {
                return std::nullopt;
            }
            const auto entry = found->second;
            std::optional<Value> value(std::move(entry->value));
            cost_ -= entry->cost;
            index_.erase(found);
            entries_.erase(entry);
            return value;
        }

        bool erase(const Key& key)
        {
            return take(key).has_value();
        }

        void setCapacity(std::size_t capacity)
        {
            capacity_ = capacity;
            evict();
        }

        void clear()
        {
            index_.clear();
            entries_.clear();
            cost_ = 0;
        }

    private:
        struct Entry
        {
            Key key;
            Value value;
            std::size_t cost;
        };

        void evict()
        {
            while (cost_ > capacity_ && !entries_.empty())
            {
                const Entry& oldest = entries_.back();
                cost_ -= oldest.cost;
                index_.erase(oldest.key);
                entries_.pop_back();
            }
        }

        std::size_t capacity_;
        std::size_t cost_ = 0;
        std::list<Entry> entries_;   // most recently used first
        std::unordered_map<Key, typename std::list<Entry>::iterator, Hash> index_;
    };
}
