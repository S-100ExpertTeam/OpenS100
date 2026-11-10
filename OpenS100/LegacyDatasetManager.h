#pragma once

#include "../SystemDatabase/Importer.h"

#include <filesystem>
#include <optional>
#include <vector>

namespace GISLibrary
{
	class SystemDatabaseSession;
}

// The viewer's Dataset Manager used to keep its own SQLite file (..\TEMP\DatasetManager.db) with
// the paths of the catalogues and datasets the user had added. This installs every file it lists
// that still exists into the System Database, once: the old file is then renamed to
// DatasetManager.db.migrated so the next start-up does not import it again.
//
// std::nullopt when there is no old file.
std::optional<std::vector<s100::sdb::ImportResult>> MigrateLegacyDatasetManager(
	GISLibrary::SystemDatabaseSession& session, const std::filesystem::path& legacyDatabase);
