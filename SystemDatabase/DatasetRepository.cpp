#include "DatasetRepository.h"

#include "Sqlite.h"

#include <string>

namespace s100::sdb
{
    namespace
    {
        constexpr std::string_view selectDataset =
            "SELECT id, name, product, product_edition, format, edition, update_number, issue_date, path, "
            "min_x, min_y, max_x, max_y, status, exchange_set_id, imported_at FROM dataset ";

        DatasetRecord read(const Statement& row)
        {
            DatasetRecord record{
                .id = row.int64(0),
                .name = row.text(1),
                .product = row.text(2),
                .productEdition = row.text(3),
                .format = parseDatasetFormat(row.text(4)).value_or(DatasetFormat::Iso8211),
                .edition = row.optionalInteger(5),
                .updateNumber = row.integer(6),
                .issueDate = row.text(7),
                .path = row.text(8),
                .bounds = std::nullopt,
                .status = parseDatasetStatus(row.text(13)).value_or(DatasetStatus::Active),
                .exchangeSetId = row.optionalInt64(14),
                .importedAt = row.text(15),
            };
            if (!row.isNull(9) && !row.isNull(10) && !row.isNull(11) && !row.isNull(12))
            {
                record.bounds = BoundingBox{ row.real(9), row.real(10), row.real(11), row.real(12) };
            }
            return record;
        }

        std::vector<DatasetRecord> readAll(Statement& statement)
        {
            std::vector<DatasetRecord> records;
            while (statement.step())
            {
                records.push_back(read(statement));
            }
            return records;
        }

        // Binds every column except id, starting at parameter 1, in the order of the INSERT and
        // UPDATE statements below.
        void bindColumns(Statement& statement, const DatasetRecord& record)
        {
            statement.bind(1, record.name).bind(2, record.product).bind(3, record.productEdition)
                .bind(4, toString(record.format)).bind(5, record.edition).bind(6, record.updateNumber)
                .bind(7, record.issueDate).bind(8, record.path);
            if (record.bounds)
            {
                statement.bind(9, record.bounds->minX).bind(10, record.bounds->minY)
                    .bind(11, record.bounds->maxX).bind(12, record.bounds->maxY);
            }
            else
            {
                statement.bindNull(9).bindNull(10).bindNull(11).bindNull(12);
            }
            statement.bind(13, toString(record.status)).bind(14, record.exchangeSetId).bind(15, record.importedAt);
        }

        std::vector<RecordId> readIds(Statement& statement)
        {
            std::vector<RecordId> ids;
            while (statement.step())
            {
                ids.push_back(statement.int64(0));
            }
            return ids;
        }
    }

    RecordId DatasetRepository::add(const DatasetRecord& record)
    {
        Statement statement(*connection_,
            "INSERT INTO dataset (name, product, product_edition, format, edition, update_number, issue_date, path, "
            "min_x, min_y, max_x, max_y, status, exchange_set_id, imported_at) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        bindColumns(statement, record);
        statement.run();
        return connection_->lastInsertRowId();
    }

    bool DatasetRepository::update(const DatasetRecord& record)
    {
        Statement statement(*connection_,
            "UPDATE dataset SET name = ?, product = ?, product_edition = ?, format = ?, edition = ?, update_number = ?, "
            "issue_date = ?, path = ?, min_x = ?, min_y = ?, max_x = ?, max_y = ?, status = ?, exchange_set_id = ?, "
            "imported_at = ? WHERE id = ?");
        bindColumns(statement, record);
        statement.bind(16, record.id);
        statement.run();
        return connection_->changes() > 0;
    }

    bool DatasetRepository::remove(RecordId id)
    {
        Statement statement(*connection_, "DELETE FROM dataset WHERE id = ?");
        statement.bind(1, id);
        statement.run();
        return connection_->changes() > 0;
    }

    std::optional<DatasetRecord> DatasetRepository::find(RecordId id) const
    {
        Statement statement(*connection_, std::string(selectDataset) + "WHERE id = ?");
        statement.bind(1, id);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::optional<DatasetRecord> DatasetRepository::find(std::string_view product, std::string_view name) const
    {
        Statement statement(*connection_, std::string(selectDataset) + "WHERE product = ? AND name = ?");
        statement.bind(1, product).bind(2, name);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::vector<DatasetRecord> DatasetRepository::all() const
    {
        Statement statement(*connection_, std::string(selectDataset) + "ORDER BY product, name");
        return readAll(statement);
    }

    std::vector<DatasetRecord> DatasetRepository::inExchangeSet(RecordId exchangeSetId) const
    {
        Statement statement(*connection_, std::string(selectDataset) + "WHERE exchange_set_id = ? ORDER BY product, name");
        statement.bind(1, exchangeSetId);
        return readAll(statement);
    }

    void DatasetRepository::addUpdate(const DatasetUpdateRecord& record)
    {
        Statement statement(*connection_,
            "INSERT INTO dataset_update (dataset_id, update_number, path, issue_date, applied) VALUES (?, ?, ?, ?, ?)");
        statement.bind(1, record.datasetId).bind(2, record.updateNumber).bind(3, record.path)
            .bind(4, record.issueDate).bind(5, record.applied ? 1 : 0);
        statement.run();
    }

    bool DatasetRepository::markUpdateApplied(RecordId datasetId, int updateNumber)
    {
        Statement statement(*connection_,
            "UPDATE dataset_update SET applied = 1 WHERE dataset_id = ? AND update_number = ?");
        statement.bind(1, datasetId).bind(2, updateNumber);
        statement.run();
        return connection_->changes() > 0;
    }

    int DatasetRepository::removeUpdates(RecordId datasetId)
    {
        Statement statement(*connection_, "DELETE FROM dataset_update WHERE dataset_id = ?");
        statement.bind(1, datasetId);
        statement.run();
        return connection_->changes();
    }

    std::vector<DatasetUpdateRecord> DatasetRepository::updates(RecordId datasetId) const
    {
        Statement statement(*connection_,
            "SELECT dataset_id, update_number, path, issue_date, applied FROM dataset_update "
            "WHERE dataset_id = ? ORDER BY update_number");
        statement.bind(1, datasetId);
        std::vector<DatasetUpdateRecord> records;
        while (statement.step())
        {
            records.push_back(DatasetUpdateRecord{
                .datasetId = statement.int64(0),
                .updateNumber = statement.integer(1),
                .path = statement.text(2),
                .issueDate = statement.text(3),
                .applied = statement.integer(4) != 0,
            });
        }
        return records;
    }

    void DatasetRepository::linkSupportFile(RecordId datasetId, RecordId supportFileId)
    {
        Statement statement(*connection_,
            "INSERT OR IGNORE INTO dataset_support_file (dataset_id, support_file_id) VALUES (?, ?)");
        statement.bind(1, datasetId).bind(2, supportFileId);
        statement.run();
    }

    bool DatasetRepository::unlinkSupportFile(RecordId datasetId, RecordId supportFileId)
    {
        Statement statement(*connection_,
            "DELETE FROM dataset_support_file WHERE dataset_id = ? AND support_file_id = ?");
        statement.bind(1, datasetId).bind(2, supportFileId);
        statement.run();
        return connection_->changes() > 0;
    }

    std::vector<RecordId> DatasetRepository::supportFileIds(RecordId datasetId) const
    {
        Statement statement(*connection_,
            "SELECT support_file_id FROM dataset_support_file WHERE dataset_id = ? ORDER BY support_file_id");
        statement.bind(1, datasetId);
        return readIds(statement);
    }

    std::vector<RecordId> DatasetRepository::datasetsReferencing(RecordId supportFileId) const
    {
        Statement statement(*connection_,
            "SELECT dataset_id FROM dataset_support_file WHERE support_file_id = ? ORDER BY dataset_id");
        statement.bind(1, supportFileId);
        return readIds(statement);
    }
}
