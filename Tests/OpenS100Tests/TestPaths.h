#pragma once
#include <filesystem>
#include <string>

// Locations used by the tests.
//
// repoRoot(): the OpenS100 checkout. Derived from the test executable, which is
//             built to <repo>/<Config>_<Platform>/, so it is the parent of the exe dir.
// tdsRoot():  external reference data. Taken from S100_TDS_ROOT, default
//             D:\s100-reference\tds (see Documents/TechDocs/DIRECTIVES.md). Tests that
//             need it call GTEST_SKIP() when the file is missing, so the suite still
//             passes on machines without the data.
namespace testpaths
{
	std::filesystem::path repoRoot();
	std::filesystem::path tdsRoot();
	std::filesystem::path korS101Cell();          // smallest cell of the Korean S-101 exchange set
	std::filesystem::path korS101Catalog();       // KOR_TDS/S100_ROOT/CATALOG.XML
	std::filesystem::path korS124Gml();           // KOR_TDS/KOR_S-124/S-124/124KR.KCG.XI.S100.TEST1.gml
	std::filesystem::path tempDir();              // per-run scratch directory
}
