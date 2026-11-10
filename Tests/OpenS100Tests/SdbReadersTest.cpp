#include <gtest/gtest.h>
#include "TestPaths.h"
#include "SdbReaders.h"
#include "SystemDatabase/Importer.h"
#include "SystemDatabase/ResourceStore.h"
#include "SystemDatabase/SystemDatabase.h"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

// System Database work item SDB-3: the readers GISLibrary gives the importer, on the Korean S-101
// exchange set (skipped when the reference data is absent) and on the catalogues in ProgramData.
using namespace s100::sdb;
namespace fs = std::filesystem;

namespace
{
	bool inKorea(const Ring& ring)
	{
		return std::ranges::all_of(ring, [](const Position& p) { return p.lon > 110 && p.lon < 150 && p.lat > 20 && p.lat < 50; });
	}
}

TEST(SdbReaders, ReadsFeatureAndPortrayalCataloguesFromTheirRoots)
{
	const GISLibrary::XmlCatalogueInspector inspector;
	const auto fc = inspector.inspectFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml");
	ASSERT_TRUE(fc);
	EXPECT_EQ(fc->product, "S-101");
	EXPECT_EQ(fc->version, "2.0.0");
	EXPECT_FALSE(fc->issueDate.empty());

	const auto pc = inspector.inspectPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml");
	ASSERT_TRUE(pc);
	EXPECT_EQ(pc->product, "S-101");
	EXPECT_EQ(pc->version, "2.0.0");

	// Each refuses the other kind.
	EXPECT_FALSE(inspector.inspectFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml"));
	EXPECT_FALSE(inspector.inspectPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml"));
}

class SdbReadersKorTds : public ::testing::Test
{
protected:
	void SetUp() override
	{
		if (!fs::exists(testpaths::korS101Catalog()))
		{
			GTEST_SKIP() << "reference exchange set not found: " << testpaths::korS101Catalog();
		}
	}
};

// The S-102 grid of the KOR test data: an HDF5 dataset, not a support file.
TEST(SdbReaders, InspectsS102GridFromItsRootAttributes)
{
	const fs::path grid = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	if (!fs::exists(grid))
	{
		GTEST_SKIP() << "S-102 test grid not found: " << grid;
	}
	const GISLibrary::Hdf5DatasetInspector inspector;
	ASSERT_TRUE(inspector.accepts(grid));
	EXPECT_FALSE(inspector.accepts(testpaths::korS101Cell()));

	const auto dataset = inspector.inspect(grid);
	ASSERT_TRUE(dataset);
	EXPECT_EQ(dataset->name, "102KR00T4G3B3000026");
	EXPECT_EQ(dataset->product, "S-102");
	EXPECT_FALSE(dataset->productEdition.empty());
	EXPECT_EQ(dataset->format, s100::sdb::DatasetFormat::Hdf5);
	ASSERT_TRUE(dataset->bounds);
	EXPECT_LT(dataset->bounds->minX, dataset->bounds->maxX);
	EXPECT_LT(dataset->bounds->minY, dataset->bounds->maxY);
	ASSERT_EQ(dataset->coverages.size(), 1u);
	EXPECT_FALSE(dataset->coverages[0].optimumDisplayScale);
	std::cout << "[ grid ] " << dataset->product << " " << dataset->productEdition << " issued " << dataset->issueDate << "\n";

	// A file that is not HDF5 is not a dataset, and says nothing on stderr.
	EXPECT_FALSE(inspector.inspect(testpaths::korS101Cell()));
}

TEST_F(SdbReadersKorTds, InspectsS101CellWithoutCatalogue)
{
	const GISLibrary::S101DatasetInspector inspector;
	ASSERT_TRUE(inspector.accepts(testpaths::korS101Cell()));
	EXPECT_FALSE(inspector.accepts(testpaths::korS101Catalog()));

	const auto dataset = inspector.inspect(testpaths::korS101Cell());
	ASSERT_TRUE(dataset);
	EXPECT_EQ(dataset->name, "101KR003G2M00");
	EXPECT_EQ(dataset->product, "S-101");
	EXPECT_EQ(dataset->productEdition, "2.0");
	EXPECT_FALSE(dataset->isUpdate);
	EXPECT_TRUE(dataset->edition.has_value());
	EXPECT_EQ(dataset->issueDate.size(), 10u);
	ASSERT_FALSE(dataset->coverages.empty());
	for (const auto& coverage : dataset->coverages)
	{
		EXPECT_TRUE(coverage.optimumDisplayScale.has_value() || coverage.maximumDisplayScale.has_value());
		ASSERT_FALSE(coverage.rings.empty());
		EXPECT_TRUE(inKorea(coverage.rings.front()));
		std::cout << "[ coverage ] optimum " << coverage.optimumDisplayScale.value_or(-1)
			<< " minimum " << coverage.minimumDisplayScale.value_or(-1)
			<< " maximum " << coverage.maximumDisplayScale.value_or(-1)
			<< " rings " << coverage.rings.size() << " points " << coverage.rings.front().size() << "\n";
	}
}

TEST_F(SdbReadersKorTds, ReadsExchangeCatalogue)
{
	const GISLibrary::ExchangeCatalogueReader reader;
	const auto set = reader.read(testpaths::korS101Catalog());
	ASSERT_TRUE(set);
	EXPECT_FALSE(set->identifier.empty());
	EXPECT_EQ(set->datasets.size(), 808u);
	EXPECT_EQ(set->supportFiles.size(), 22u);

	const auto& first = set->datasets.front();
	EXPECT_TRUE(fs::exists(first.file)) << first.file;
	EXPECT_EQ(first.product, "S-101");
	EXPECT_EQ(first.productEdition, "2.0.0");
	ASSERT_FALSE(first.coverages.empty());
	EXPECT_TRUE(inKorea(first.coverages.front().rings.front()));

	const auto& support = set->supportFiles.front();
	EXPECT_TRUE(fs::exists(support.file)) << support.file;
	EXPECT_EQ(support.product, "S-101");
	EXPECT_EQ(support.purpose, "new");
}

TEST_F(SdbReadersKorTds, ImportsOneCellThroughTheRealReaders)
{
	const auto root = testpaths::tempDir() / L"SdbReaders" / L"one-cell";
	fs::remove_all(root);
	ResourceStore store(root);
	auto database = SystemDatabase::open(store.databaseFile());
	const GISLibrary::S101DatasetInspector cells;
	const GISLibrary::XmlCatalogueInspector catalogues;
	const GISLibrary::ExchangeCatalogueReader exchangeSets;
	Importer importer(database, store, Importer::Readers{ { &cells }, &catalogues, &exchangeSets });

	const auto imported = importer.importDataset(testpaths::korS101Cell());

	ASSERT_EQ(imported.outcome, ImportOutcome::Added) << imported.message;
	EXPECT_EQ(database.datasets().find(imported.id)->path, "DATASET/S-101/101KR003G2M00/101KR003G2M00.000");
	EXPECT_FALSE(database.coverages().forDataset(imported.id).empty());
	EXPECT_TRUE(store.exists("DATASET/S-101/101KR003G2M00/101KR003G2M00.000"));
}

// The whole Korean exchange set (808 cells). Slow, so only when S100_SDB_FULL_IMPORT is set; it
// prints the time taken and leaves the store in the test scratch folder for inspection.
TEST_F(SdbReadersKorTds, ImportsWholeExchangeSetWhenAsked)
{
	char* flag = nullptr;
	std::size_t length = 0;
	const bool asked = _dupenv_s(&flag, &length, "S100_SDB_FULL_IMPORT") == 0 && flag;
	std::free(flag);
	if (!asked)
	{
		GTEST_SKIP() << "set S100_SDB_FULL_IMPORT=1 to import all 808 cells";
	}
	const auto root = testpaths::tempDir() / L"SdbReaders" / L"KOR_TDS";
	fs::remove_all(root);
	ResourceStore store(root);
	auto database = SystemDatabase::open(store.databaseFile());
	const GISLibrary::S101DatasetInspector cells;
	const GISLibrary::XmlCatalogueInspector catalogues;
	const GISLibrary::ExchangeCatalogueReader exchangeSets;
	Importer importer(database, store, Importer::Readers{ { &cells }, &catalogues, &exchangeSets });

	// With the S-101 catalogues, the store is a complete one a viewer can open
	// (set OPENS100_SDB_ROOT to the root printed below).
	ASSERT_EQ(importer.importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").outcome, ImportOutcome::Added);
	ASSERT_EQ(importer.importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal").outcome, ImportOutcome::Added);

	const auto start = std::chrono::steady_clock::now();
	const auto report = importer.importExchangeSet(testpaths::korS101Catalog());
	const auto seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();

	std::cout << "[ timing ] KOR_TDS import " << seconds << " s, root " << root.string() << "\n";
	for (const auto& result : report.results)
	{
		if (result.outcome == ImportOutcome::Rejected || !result.message.starts_with("new"))
		{
			std::cout << "[ result ] " << toString(result.outcome) << " " << result.source.filename().string() << ": " << result.message << "\n";
		}
	}
	EXPECT_EQ(report.count(ImportOutcome::Added), 808u + 22u);
	EXPECT_EQ(report.count(ImportOutcome::Rejected), 0u);
	EXPECT_TRUE(report.installProblems.empty());
	EXPECT_EQ(database.datasets().all().size(), 808u);
	EXPECT_GE(database.coverages().active().size(), 808u);
}
