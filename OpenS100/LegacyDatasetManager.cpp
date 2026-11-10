#include "pch.h"
#include "LegacyDatasetManager.h"

#include "../GISLibrary/SystemDatabaseSession.h"
#include "../SystemDatabase/Sqlite.h"
#include "../SystemDatabase/Utf8Path.h"

namespace
{
	// The filePath column of a table of the old database; empty when the table is missing (the
	// old SupportFiles table never got created).
	std::vector<std::filesystem::path> listedFiles(s100::sdb::Connection& connection, const char* table)
	{
		std::vector<std::filesystem::path> files;
		try
		{
			s100::sdb::Statement statement(connection, std::string("SELECT filePath FROM ") + table);
			while (statement.step())
			{
				auto file = s100::sdb::fromUtf8(statement.text(0));
				std::error_code error;
				if (std::filesystem::exists(file, error))
				{
					files.push_back(std::move(file));
				}
			}
		}
		catch (const s100::sdb::DatabaseError&)
		{
		}
		return files;
	}
}

std::optional<std::vector<s100::sdb::ImportResult>> MigrateLegacyDatasetManager(
	GISLibrary::SystemDatabaseSession& session, const std::filesystem::path& legacyDatabase)
{
	std::error_code error;
	if (!std::filesystem::is_regular_file(legacyDatabase, error))
	{
		return std::nullopt;
	}

	std::vector<std::filesystem::path> featureCatalogues;
	std::vector<std::filesystem::path> portrayalCatalogues;
	std::vector<std::filesystem::path> datasets;
	std::vector<std::filesystem::path> supportFiles;
	{
		auto connection = s100::sdb::Connection::open(legacyDatabase);
		featureCatalogues = listedFiles(connection, "FeatureCatalogue");
		portrayalCatalogues = listedFiles(connection, "PortrayalCatalogue");
		datasets = listedFiles(connection, "DatasetFiles");
		supportFiles = listedFiles(connection, "SupportFiles");
	}

	auto& importer = session.importer();
	std::vector<s100::sdb::ImportResult> results;
	for (const auto& file : featureCatalogues)
	{
		results.push_back(importer.importFeatureCatalogue(file));
	}
	for (const auto& file : portrayalCatalogues)
	{
		results.push_back(importer.importPortrayalCatalogue(file));
	}
	for (const auto& file : supportFiles)
	{
		results.push_back(importer.importSupportFile(file, "", "new"));
	}
	for (const auto& file : datasets)
	{
		results.push_back(importer.importDataset(file));
	}

	auto migrated = legacyDatabase;
	migrated += ".migrated";
	std::filesystem::rename(legacyDatabase, migrated, error);
	return results;
}
