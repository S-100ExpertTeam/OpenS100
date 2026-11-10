#include "ExchangeSetRepository.h"

#include "Sqlite.h"

namespace s100::sdb
{
    namespace
    {
        ExchangeSetRecord read(const Statement& row)
        {
            return ExchangeSetRecord{
                .id = row.int64(0),
                .sourcePath = row.text(1),
                .identifier = row.text(2),
                .issueDate = row.text(3),
                .importedAt = row.text(4),
            };
        }
    }

    RecordId ExchangeSetRepository::add(const ExchangeSetRecord& record)
    {
        Statement statement(*connection_,
            "INSERT INTO exchange_set (source_path, identifier, issue_date, imported_at) VALUES (?, ?, ?, ?)");
        statement.bind(1, record.sourcePath).bind(2, record.identifier).bind(3, record.issueDate).bind(4, record.importedAt);
        statement.run();
        return connection_->lastInsertRowId();
    }

    bool ExchangeSetRepository::remove(RecordId id)
    {
        Statement statement(*connection_, "DELETE FROM exchange_set WHERE id = ?");
        statement.bind(1, id);
        statement.run();
        return connection_->changes() > 0;
    }

    std::optional<ExchangeSetRecord> ExchangeSetRepository::find(RecordId id) const
    {
        Statement statement(*connection_,
            "SELECT id, source_path, identifier, issue_date, imported_at FROM exchange_set WHERE id = ?");
        statement.bind(1, id);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::vector<ExchangeSetRecord> ExchangeSetRepository::all() const
    {
        Statement statement(*connection_,
            "SELECT id, source_path, identifier, issue_date, imported_at FROM exchange_set ORDER BY imported_at, id");
        std::vector<ExchangeSetRecord> records;
        while (statement.step())
        {
            records.push_back(read(statement));
        }
        return records;
    }
}
