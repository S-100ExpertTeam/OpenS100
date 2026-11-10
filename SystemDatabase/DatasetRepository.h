#pragma once

#include "Records.h"

#include <optional>
#include <string_view>
#include <vector>

namespace s100::sdb
{
    class Connection;

    // Datasets, their update files and the support files they reference. Removing a dataset
    // removes its updates, coverages and support-file links (not the support files themselves).
    class DatasetRepository final
    {
    public:
        explicit DatasetRepository(Connection& connection) noexcept : connection_(&connection) {}

        // Throws DatabaseError (SQLITE_CONSTRAINT_UNIQUE) when (product, name) or the path is
        // already registered.
        RecordId add(const DatasetRecord& record);
        bool update(const DatasetRecord& record);
        bool remove(RecordId id);

        [[nodiscard]] std::optional<DatasetRecord> find(RecordId id) const;
        [[nodiscard]] std::optional<DatasetRecord> find(std::string_view product, std::string_view name) const;
        // Ordered by product, then name.
        [[nodiscard]] std::vector<DatasetRecord> all() const;
        [[nodiscard]] std::vector<DatasetRecord> inExchangeSet(RecordId exchangeSetId) const;

        // Update files (.001, .002, ...). A dataset has at most one row per update number.
        void addUpdate(const DatasetUpdateRecord& record);
        bool markUpdateApplied(RecordId datasetId, int updateNumber);
        // Removes every update row of the dataset (after a new edition replaced it).
        int removeUpdates(RecordId datasetId);
        // Ordered by update number.
        [[nodiscard]] std::vector<DatasetUpdateRecord> updates(RecordId datasetId) const;

        // Links are idempotent: linking twice keeps one link.
        void linkSupportFile(RecordId datasetId, RecordId supportFileId);
        bool unlinkSupportFile(RecordId datasetId, RecordId supportFileId);
        [[nodiscard]] std::vector<RecordId> supportFileIds(RecordId datasetId) const;
        [[nodiscard]] std::vector<RecordId> datasetsReferencing(RecordId supportFileId) const;

    private:
        Connection* connection_;
    };
}
