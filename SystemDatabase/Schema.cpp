#include "Schema.h"

#include "Sqlite.h"

#include <array>
#include <string>
#include <string_view>

namespace s100::sdb::schema
{
    namespace
    {
        // migrations[n] takes the database from version n to n + 1. Never edit a released step;
        // append a new one and bump CurrentVersion.
        constexpr std::array<std::string_view, CurrentVersion> migrations = {
            // 0 -> 1
            R"sql(
CREATE TABLE exchange_set (
  id INTEGER PRIMARY KEY,
  source_path TEXT NOT NULL,
  identifier TEXT NOT NULL DEFAULT '',
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL
);

CREATE TABLE feature_catalogue (
  id INTEGER PRIMARY KEY,
  product TEXT NOT NULL,
  version TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  sha256 TEXT NOT NULL,
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL,
  UNIQUE (product, version)
);

CREATE TABLE portrayal_catalogue (
  id INTEGER PRIMARY KEY,
  product TEXT NOT NULL,
  version TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  sha256 TEXT NOT NULL,
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL,
  UNIQUE (product, version)
);

CREATE TABLE dataset (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  product TEXT NOT NULL,
  product_edition TEXT NOT NULL DEFAULT '',
  format TEXT NOT NULL CHECK (format IN ('iso8211', 'gml', 'hdf5')),
  edition INTEGER,
  update_number INTEGER NOT NULL DEFAULT 0,
  issue_date TEXT NOT NULL DEFAULT '',
  path TEXT NOT NULL UNIQUE,
  min_x REAL, min_y REAL, max_x REAL, max_y REAL,
  status TEXT NOT NULL DEFAULT 'active' CHECK (status IN ('active', 'cancelled')),
  exchange_set_id INTEGER REFERENCES exchange_set(id) ON DELETE SET NULL,
  imported_at TEXT NOT NULL,
  UNIQUE (product, name)
);
CREATE INDEX dataset_exchange_set ON dataset(exchange_set_id);

CREATE TABLE dataset_update (
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  update_number INTEGER NOT NULL,
  path TEXT NOT NULL UNIQUE,
  issue_date TEXT NOT NULL DEFAULT '',
  applied INTEGER NOT NULL DEFAULT 0 CHECK (applied IN (0, 1)),
  PRIMARY KEY (dataset_id, update_number)
);

CREATE TABLE data_coverage (
  id INTEGER PRIMARY KEY,
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  optimum_display_scale INTEGER,
  minimum_display_scale INTEGER,
  maximum_display_scale INTEGER,
  min_x REAL NOT NULL, min_y REAL NOT NULL, max_x REAL NOT NULL, max_y REAL NOT NULL,
  polygon BLOB NOT NULL
);
CREATE INDEX data_coverage_dataset ON data_coverage(dataset_id);

CREATE TABLE support_file (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  purpose TEXT NOT NULL DEFAULT '',
  edition INTEGER,
  format TEXT NOT NULL DEFAULT '',
  issue_date TEXT NOT NULL DEFAULT '',
  sha256 TEXT NOT NULL,
  imported_at TEXT NOT NULL
);

CREATE TABLE dataset_support_file (
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  support_file_id INTEGER NOT NULL REFERENCES support_file(id) ON DELETE CASCADE,
  PRIMARY KEY (dataset_id, support_file_id)
);
CREATE INDEX dataset_support_file_reverse ON dataset_support_file(support_file_id);
)sql",
        };
    }

    int version(Connection& connection)
    {
        Statement statement(connection, "PRAGMA user_version");
        return statement.step() ? statement.integer(0) : 0;
    }

    void migrate(Connection& connection)
    {
        const int current = version(connection);
        if (current > CurrentVersion)
        {
            throw DatabaseError(0, "System Database schema version " + std::to_string(current) +
                " is newer than this library supports (" + std::to_string(CurrentVersion) + ")");
        }

        for (int step = current; step < CurrentVersion; ++step)
        {
            Transaction transaction(connection);
            connection.execute(migrations[static_cast<std::size_t>(step)]);
            connection.execute("PRAGMA user_version = " + std::to_string(step + 1));
            transaction.commit();
        }
    }

    void configure(Connection& connection, bool inMemory)
    {
        connection.execute("PRAGMA foreign_keys = ON");
        if (!inMemory)
        {
            // WAL lets a viewer keep reading while an import writes.
            connection.execute("PRAGMA journal_mode = WAL");
            connection.execute("PRAGMA synchronous = NORMAL");
        }
    }
}
