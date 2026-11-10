#pragma once

#include "Records.h"

#include <optional>
#include <vector>

namespace s100::sdb
{
    class Connection;

    // Exchange sets that datasets were imported from. Removing one keeps its datasets and clears
    // their exchangeSetId.
    class ExchangeSetRepository final
    {
    public:
        explicit ExchangeSetRepository(Connection& connection) noexcept : connection_(&connection) {}

        RecordId add(const ExchangeSetRecord& record);
        bool remove(RecordId id);

        [[nodiscard]] std::optional<ExchangeSetRecord> find(RecordId id) const;
        // Ordered by import time, oldest first.
        [[nodiscard]] std::vector<ExchangeSetRecord> all() const;

    private:
        Connection* connection_;
    };
}
