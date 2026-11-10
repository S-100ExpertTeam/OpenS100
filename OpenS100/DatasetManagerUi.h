#pragma once

#include "../SystemDatabase/Importer.h"

#include <filesystem>
#include <string>
#include <vector>

class CWnd;

namespace GISLibrary
{
	class SystemDatabaseSession;
}

// Helpers shared by the Dataset Manager tabs, which are thin views of the System Database
// (Documents/TechDocs/2026-11-10-system-database.md, SDB-7).
namespace DatasetManagerUi
{
	// The application's System Database; null (after telling the user) when it could not be opened.
	GISLibrary::SystemDatabaseSession* Session(CWnd* parent);

	// One message box summing up what an import did, listing the files that were not installed.
	void Report(CWnd* parent, const std::vector<s100::sdb::ImportResult>& results, const std::vector<std::string>& problems = {});

	// Asks before removing installed resources; true to go ahead.
	bool ConfirmRemoval(CWnd* parent, std::size_t count);

	// Shows the file in Explorer.
	void ShowInFolder(const std::filesystem::path& file);

	// The paths dropped on a window.
	std::vector<std::filesystem::path> DroppedFiles(void* dropInfo);

	std::wstring Widen(const std::string& utf8);
}
