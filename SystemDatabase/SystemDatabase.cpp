#include "SystemDatabase.h"

#include "Schema.h"

#include <system_error>

namespace s100::sdb
{
    SystemDatabase SystemDatabase::open(const std::filesystem::path& file)
    {
        if (file.has_parent_path())
        {
            std::error_code error;
            std::filesystem::create_directories(file.parent_path(), error);
            if (error)
            {
                const std::u8string name = file.parent_path().u8string();
                throw DatabaseError(0, "cannot create directory '" + std::string(name.begin(), name.end()) +
                    "': " + error.message());
            }
        }

        auto connection = std::make_unique<Connection>(Connection::open(file));
        schema::configure(*connection, false);
        schema::migrate(*connection);
        return SystemDatabase(std::move(connection));
    }

    SystemDatabase SystemDatabase::openInMemory()
    {
        auto connection = std::make_unique<Connection>(Connection::openInMemory());
        schema::configure(*connection, true);
        schema::migrate(*connection);
        return SystemDatabase(std::move(connection));
    }

    SystemDatabase::SystemDatabase(std::unique_ptr<Connection> connection)
        : connection_(std::move(connection)),
          exchangeSets_(*connection_),
          featureCatalogues_(*connection_, CatalogueKind::Feature),
          portrayalCatalogues_(*connection_, CatalogueKind::Portrayal),
          datasets_(*connection_),
          coverages_(*connection_),
          supportFiles_(*connection_)
    {
    }

    int SystemDatabase::schemaVersion() const
    {
        return schema::version(*connection_);
    }
}
