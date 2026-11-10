#include "CoverageRepository.h"

#include "Sqlite.h"

namespace s100::sdb
{
    namespace
    {
        DataCoverageRecord read(const Statement& row)
        {
            return DataCoverageRecord{
                .id = row.int64(0),
                .datasetId = row.int64(1),
                .optimumDisplayScale = row.optionalInteger(2),
                .minimumDisplayScale = row.optionalInteger(3),
                .maximumDisplayScale = row.optionalInteger(4),
                .bounds = BoundingBox{ row.real(5), row.real(6), row.real(7), row.real(8) },
                .polygon = row.blob(9),
            };
        }

        std::vector<DataCoverageRecord> readAll(Statement& statement)
        {
            std::vector<DataCoverageRecord> records;
            while (statement.step())
            {
                records.push_back(read(statement));
            }
            return records;
        }
    }

    std::vector<RecordId> CoverageRepository::replace(RecordId datasetId, std::span<const DataCoverageRecord> coverages)
    {
        Statement remove(*connection_, "DELETE FROM data_coverage WHERE dataset_id = ?");
        remove.bind(1, datasetId);
        remove.run();

        Statement insert(*connection_,
            "INSERT INTO data_coverage (dataset_id, optimum_display_scale, minimum_display_scale, maximum_display_scale, "
            "min_x, min_y, max_x, max_y, polygon) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
        std::vector<RecordId> ids;
        ids.reserve(coverages.size());
        for (const auto& coverage : coverages)
        {
            insert.reset();
            insert.bind(1, datasetId).bind(2, coverage.optimumDisplayScale).bind(3, coverage.minimumDisplayScale)
                .bind(4, coverage.maximumDisplayScale).bind(5, coverage.bounds.minX).bind(6, coverage.bounds.minY)
                .bind(7, coverage.bounds.maxX).bind(8, coverage.bounds.maxY)
                .bind(9, std::span<const std::uint8_t>(coverage.polygon));
            insert.run();
            ids.push_back(connection_->lastInsertRowId());
        }
        return ids;
    }

    std::vector<DataCoverageRecord> CoverageRepository::forDataset(RecordId datasetId) const
    {
        Statement statement(*connection_,
            "SELECT id, dataset_id, optimum_display_scale, minimum_display_scale, maximum_display_scale, "
            "min_x, min_y, max_x, max_y, polygon FROM data_coverage WHERE dataset_id = ? ORDER BY id");
        statement.bind(1, datasetId);
        return readAll(statement);
    }

    std::vector<DataCoverageRecord> CoverageRepository::active() const
    {
        Statement statement(*connection_,
            "SELECT c.id, c.dataset_id, c.optimum_display_scale, c.minimum_display_scale, c.maximum_display_scale, "
            "c.min_x, c.min_y, c.max_x, c.max_y, c.polygon FROM data_coverage c "
            "JOIN dataset d ON d.id = c.dataset_id WHERE d.status = 'active' ORDER BY c.dataset_id, c.id");
        return readAll(statement);
    }
}
