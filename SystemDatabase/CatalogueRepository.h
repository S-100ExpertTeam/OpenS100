#pragma once

#include "Records.h"

#include <optional>
#include <string_view>
#include <vector>

namespace s100::sdb
{
    class Connection;

    enum class CatalogueKind
    {
        Feature,
        Portrayal,
    };

    // Feature catalogues or portrayal catalogues, one per (product, version).
    class CatalogueRepository final
    {
    public:
        CatalogueRepository(Connection& connection, CatalogueKind kind) noexcept
            : connection_(&connection), kind_(kind) {}

        [[nodiscard]] CatalogueKind kind() const noexcept { return kind_; }

        // Inserts the record and returns its id. Throws DatabaseError (SQLITE_CONSTRAINT_UNIQUE)
        // when the (product, version) or the path is already registered.
        RecordId add(const CatalogueRecord& record);
        // Overwrites every column of the row with record.id. Returns false when there is none.
        bool update(const CatalogueRecord& record);
        bool remove(RecordId id);

        [[nodiscard]] std::optional<CatalogueRecord> find(RecordId id) const;
        [[nodiscard]] std::optional<CatalogueRecord> find(std::string_view product, std::string_view version) const;
        // Ordered by product, then version as text.
        [[nodiscard]] std::vector<CatalogueRecord> all() const;
        [[nodiscard]] std::vector<CatalogueRecord> forProduct(std::string_view product) const;

    private:
        [[nodiscard]] std::string_view table() const noexcept;

        Connection* connection_;
        CatalogueKind kind_;
    };
}
