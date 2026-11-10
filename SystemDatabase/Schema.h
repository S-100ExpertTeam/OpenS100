#pragma once

namespace s100::sdb
{
    class Connection;

    // Schema of the System Database. The version lives in PRAGMA user_version.
    namespace schema
    {
        inline constexpr int CurrentVersion = 1;

        // Reads PRAGMA user_version; 0 for a database that has never been initialised.
        [[nodiscard]] int version(Connection& connection);

        // Brings the database up to CurrentVersion, one migration step per version, each in its
        // own transaction. Throws DatabaseError when the database is newer than this library.
        void migrate(Connection& connection);

        // Per-connection settings that SQLite does not store in the file (foreign keys, ...).
        void configure(Connection& connection, bool inMemory);
    }
}
