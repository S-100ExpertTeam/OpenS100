#pragma once

#include "Records.h"

#include <span>
#include <vector>

namespace s100::sdb
{
    class Connection;

    // DataCoverage polygons and display scales of the datasets: the input of the S-98 E-1 dataset
    // selection, read once at start-up so no dataset file has to be opened to decide what to load.
    class CoverageRepository final
    {
    public:
        explicit CoverageRepository(Connection& connection) noexcept : connection_(&connection) {}

        // Replaces every coverage of the dataset with `coverages` (their id and datasetId are
        // ignored). Returns the new ids in input order.
        std::vector<RecordId> replace(RecordId datasetId, std::span<const DataCoverageRecord> coverages);

        // In insertion order.
        [[nodiscard]] std::vector<DataCoverageRecord> forDataset(RecordId datasetId) const;
        // Coverages of every active (not cancelled) dataset, ordered by dataset id then insertion.
        [[nodiscard]] std::vector<DataCoverageRecord> active() const;

    private:
        Connection* connection_;
    };
}
