#pragma once

#include "Records.h"

#include <optional>
#include <string_view>
#include <vector>

namespace s100::sdb
{
    class Connection;

    // Support files (pictures, text, ...). Removing one also removes its dataset links.
    class SupportFileRepository final
    {
    public:
        explicit SupportFileRepository(Connection& connection) noexcept : connection_(&connection) {}

        // Throws DatabaseError (SQLITE_CONSTRAINT_UNIQUE) when the path is already registered.
        RecordId add(const SupportFileRecord& record);
        bool update(const SupportFileRecord& record);
        bool remove(RecordId id);

        [[nodiscard]] std::optional<SupportFileRecord> find(RecordId id) const;
        [[nodiscard]] std::optional<SupportFileRecord> findByPath(std::string_view path) const;
        // Ordered by path.
        [[nodiscard]] std::vector<SupportFileRecord> all() const;

    private:
        Connection* connection_;
    };
}
