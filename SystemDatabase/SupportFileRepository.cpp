#include "SupportFileRepository.h"

#include "Sqlite.h"

#include <string>

namespace s100::sdb
{
    namespace
    {
        constexpr std::string_view selectSupportFile =
            "SELECT id, name, path, purpose, edition, format, issue_date, sha256, imported_at FROM support_file ";

        SupportFileRecord read(const Statement& row)
        {
            return SupportFileRecord{
                .id = row.int64(0),
                .name = row.text(1),
                .path = row.text(2),
                .purpose = row.text(3),
                .edition = row.optionalInteger(4),
                .format = row.text(5),
                .issueDate = row.text(6),
                .sha256 = row.text(7),
                .importedAt = row.text(8),
            };
        }

        void bindColumns(Statement& statement, const SupportFileRecord& record)
        {
            statement.bind(1, record.name).bind(2, record.path).bind(3, record.purpose).bind(4, record.edition)
                .bind(5, record.format).bind(6, record.issueDate).bind(7, record.sha256).bind(8, record.importedAt);
        }
    }

    RecordId SupportFileRepository::add(const SupportFileRecord& record)
    {
        Statement statement(*connection_,
            "INSERT INTO support_file (name, path, purpose, edition, format, issue_date, sha256, imported_at) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        bindColumns(statement, record);
        statement.run();
        return connection_->lastInsertRowId();
    }

    bool SupportFileRepository::update(const SupportFileRecord& record)
    {
        Statement statement(*connection_,
            "UPDATE support_file SET name = ?, path = ?, purpose = ?, edition = ?, format = ?, issue_date = ?, "
            "sha256 = ?, imported_at = ? WHERE id = ?");
        bindColumns(statement, record);
        statement.bind(9, record.id);
        statement.run();
        return connection_->changes() > 0;
    }

    bool SupportFileRepository::remove(RecordId id)
    {
        Statement statement(*connection_, "DELETE FROM support_file WHERE id = ?");
        statement.bind(1, id);
        statement.run();
        return connection_->changes() > 0;
    }

    std::optional<SupportFileRecord> SupportFileRepository::find(RecordId id) const
    {
        Statement statement(*connection_, std::string(selectSupportFile) + "WHERE id = ?");
        statement.bind(1, id);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::optional<SupportFileRecord> SupportFileRepository::findByPath(std::string_view path) const
    {
        Statement statement(*connection_, std::string(selectSupportFile) + "WHERE path = ?");
        statement.bind(1, path);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::vector<SupportFileRecord> SupportFileRepository::all() const
    {
        Statement statement(*connection_, std::string(selectSupportFile) + "ORDER BY path");
        std::vector<SupportFileRecord> records;
        while (statement.step())
        {
            records.push_back(read(statement));
        }
        return records;
    }
}
