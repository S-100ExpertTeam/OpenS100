#include "CatalogueRepository.h"

#include "Sqlite.h"

#include <string>

namespace s100::sdb
{
    namespace
    {
        constexpr std::string_view columns = "id, product, version, path, sha256, issue_date, imported_at";

        CatalogueRecord read(const Statement& row)
        {
            return CatalogueRecord{
                .id = row.int64(0),
                .product = row.text(1),
                .version = row.text(2),
                .path = row.text(3),
                .sha256 = row.text(4),
                .issueDate = row.text(5),
                .importedAt = row.text(6),
            };
        }

        std::vector<CatalogueRecord> readAll(Statement& statement)
        {
            std::vector<CatalogueRecord> records;
            while (statement.step())
            {
                records.push_back(read(statement));
            }
            return records;
        }

        std::string select(std::string_view table, std::string_view tail)
        {
            return "SELECT " + std::string(columns) + " FROM " + std::string(table) + " " + std::string(tail);
        }
    }

    std::string_view CatalogueRepository::table() const noexcept
    {
        return kind_ == CatalogueKind::Feature ? "feature_catalogue" : "portrayal_catalogue";
    }

    RecordId CatalogueRepository::add(const CatalogueRecord& record)
    {
        Statement statement(*connection_, "INSERT INTO " + std::string(table()) +
            " (product, version, path, sha256, issue_date, imported_at) VALUES (?, ?, ?, ?, ?, ?)");
        statement.bind(1, record.product).bind(2, record.version).bind(3, record.path)
            .bind(4, record.sha256).bind(5, record.issueDate).bind(6, record.importedAt);
        statement.run();
        return connection_->lastInsertRowId();
    }

    bool CatalogueRepository::update(const CatalogueRecord& record)
    {
        Statement statement(*connection_, "UPDATE " + std::string(table()) +
            " SET product = ?, version = ?, path = ?, sha256 = ?, issue_date = ?, imported_at = ? WHERE id = ?");
        statement.bind(1, record.product).bind(2, record.version).bind(3, record.path)
            .bind(4, record.sha256).bind(5, record.issueDate).bind(6, record.importedAt).bind(7, record.id);
        statement.run();
        return connection_->changes() > 0;
    }

    bool CatalogueRepository::remove(RecordId id)
    {
        Statement statement(*connection_, "DELETE FROM " + std::string(table()) + " WHERE id = ?");
        statement.bind(1, id);
        statement.run();
        return connection_->changes() > 0;
    }

    std::optional<CatalogueRecord> CatalogueRepository::find(RecordId id) const
    {
        Statement statement(*connection_, select(table(), "WHERE id = ?"));
        statement.bind(1, id);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::optional<CatalogueRecord> CatalogueRepository::find(std::string_view product, std::string_view version) const
    {
        Statement statement(*connection_, select(table(), "WHERE product = ? AND version = ?"));
        statement.bind(1, product).bind(2, version);
        if (!statement.step())
        {
            return std::nullopt;
        }
        return read(statement);
    }

    std::vector<CatalogueRecord> CatalogueRepository::all() const
    {
        Statement statement(*connection_, select(table(), "ORDER BY product, version"));
        return readAll(statement);
    }

    std::vector<CatalogueRecord> CatalogueRepository::forProduct(std::string_view product) const
    {
        Statement statement(*connection_, select(table(), "WHERE product = ? ORDER BY version"));
        statement.bind(1, product);
        return readAll(statement);
    }
}
