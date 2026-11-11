#include <gtest/gtest.h>
#include "TestPaths.h"
#include "SdbReaders.h"
#include "SystemDatabase/Importer.h"
#include "SystemDatabase/ResourceStore.h"
#include "SystemDatabase/SystemDatabase.h"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
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

// The S-102 portrayal catalogue as IHO delivers it: a .zip with the catalogue two folders down.
TEST(SdbReaders, InstallsDeliveredS102PortrayalCatalogueZip)
{
	const fs::path zip = testpaths::tdsRoot() / L"FC_PC" / L"S-102" / L"unzipped" / L"S-102" / L"CATALOGUES" / L"102_Portrayal_Catalogue_3.0.0.zip";
	if (!fs::exists(zip))
	{
		GTEST_SKIP() << "S-102 portrayal catalogue archive not found: " << zip;
	}
	const auto root = testpaths::tempDir() / L"SdbReaders" / L"PcZip";
	fs::remove_all(root);
	s100::sdb::ResourceStore store(root);
	auto database = s100::sdb::SystemDatabase::open(store.databaseFile());
	const GISLibrary::XmlCatalogueInspector catalogues;
	s100::sdb::Importer importer(database, store, s100::sdb::Importer::Readers{ {}, &catalogues, nullptr });

	const auto added = importer.importPortrayalCatalogue(zip);
	ASSERT_EQ(added.outcome, s100::sdb::ImportOutcome::Added) << added.message;
	const auto record = database.portrayalCatalogues().find(added.id);
	ASSERT_TRUE(record);
	EXPECT_EQ(record->product, "S-102");
	EXPECT_EQ(record->version, "3.0.0");
	EXPECT_TRUE(store.exists("PC/S-102/3.0.0/Rules/BathymetryCoverage.lua"));
	EXPECT_TRUE(store.exists("PC/S-102/3.0.0/ColorProfiles/colorProfile.xml"));
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

// S-100 Edition 4.0 catalogues name the folder in filePath and only the file in fileName.
TEST(SdbReaders, ExchangeCatalogueJoinsEdition4FilePath)
{
	const auto folder = testpaths::tempDir() / L"SdbReaders" / L"Edition4" / L"EXCH_ROOT";
	fs::remove_all(folder.parent_path());
	fs::create_directories(folder);
	{
		std::ofstream out(folder / "CATALOG.XML", std::ios::binary);
		out << R"(<?xml version="1.0" encoding="UTF-8"?>
<S100XC:S100_ExchangeCatalogue xmlns:S100XC="http://www.iho.int/s100/xc">
  <S100XC:identifier><S100XC:identifier>KR-2020</S100XC:identifier><S100XC:dateTime>2020-12-03T00:00:00Z</S100XC:dateTime></S100XC:identifier>
  <S100XC:datasetDiscoveryMetadata>
    <S100XC:S100_DatasetDiscoveryMetadata>
      <S100XC:fileName>101KR00251200.000</S100XC:fileName>
      <S100XC:filePath>DataSet/101KR00251200</S100XC:filePath>
    </S100XC:S100_DatasetDiscoveryMetadata>
  </S100XC:datasetDiscoveryMetadata>
  <S100XC:supportFileDiscoveryMetadata>
    <S100XC:S100_SupportFileDiscoveryMetadata>
      <S100XC:fileName>101KR000010E100.TXT</S100XC:fileName>
      <S100XC:filePath>S-101/SUPPORT_FILES</S100XC:filePath>
    </S100XC:S100_SupportFileDiscoveryMetadata>
  </S100XC:supportFileDiscoveryMetadata>
</S100XC:S100_ExchangeCatalogue>)";
	}
	const GISLibrary::ExchangeCatalogueReader reader;
	const auto set = reader.read(folder / "CATALOG.XML");
	ASSERT_TRUE(set);
	ASSERT_EQ(set->datasets.size(), 1u);
	EXPECT_EQ(set->datasets[0].file, (folder / "DataSet" / "101KR00251200" / "101KR00251200.000").lexically_normal());
	ASSERT_EQ(set->supportFiles.size(), 1u);
	EXPECT_EQ(set->supportFiles[0].file, (folder / "S-101" / "SUPPORT_FILES" / "101KR000010E100.TXT").lexically_normal());
	EXPECT_EQ(set->supportFiles[0].product, "S-101");
}

// S-100 Part 10b GML positions: EPSG:4326 is latitude first, CRS84 longitude first; data that
// writes longitude first under EPSG:4326 (the KOR S-124) is recognised by its latitudes.
TEST(SdbReaders, BoundsGmlPositionsInEitherAxisOrder)
{
	const auto latitudeFirst = GISLibrary::gmlPositionBounds(R"(<D xmlns:gml="http://www.opengis.net/gml/3.2">
		<gml:Curve srsName="EPSG:4326"><gml:posList>35.1 129.0 35.3 129.4</gml:posList></gml:Curve>
		<gml:Point><gml:pos>35.2 129.2 12.5</gml:pos></gml:Point></D>)");
	ASSERT_TRUE(latitudeFirst);
	EXPECT_EQ(*latitudeFirst, (BoundingBox{ 129.0, 35.1, 129.4, 35.3 }));

	const auto crs84 = GISLibrary::gmlPositionBounds(R"(<D xmlns:gml="http://www.opengis.net/gml/3.2" srsName="urn:ogc:def:crs:OGC:1.3:CRS84">
		<gml:posList>10.5 50.0 11.5 51.0</gml:posList></D>)");
	ASSERT_TRUE(crs84);
	EXPECT_EQ(*crs84, (BoundingBox{ 10.5, 50.0, 11.5, 51.0 }));

	const auto swapped = GISLibrary::gmlPositionBounds(R"(<D xmlns:gml="http://www.opengis.net/gml/3.2">
		<gml:Surface srsName="EPSG:4326"><gml:posList>129.05 35.11 129.06 35.12</gml:posList></gml:Surface></D>)");
	ASSERT_TRUE(swapped);
	EXPECT_EQ(*swapped, (BoundingBox{ 129.05, 35.11, 129.06, 35.12 }));

	// The envelope only counts when there is no geometry.
	const auto envelope = GISLibrary::gmlPositionBounds(R"(<D xmlns:gml="http://www.opengis.net/gml/3.2">
		<gml:boundedBy><gml:Envelope srsName="EPSG:4326"><gml:lowerCorner>0 0</gml:lowerCorner><gml:upperCorner>80 170</gml:upperCorner></gml:Envelope></gml:boundedBy>
		<gml:pos srsDimension="2">35 129</gml:pos></D>)");
	ASSERT_TRUE(envelope);
	EXPECT_EQ(*envelope, (BoundingBox{ 129, 35, 129, 35 }));
	EXPECT_FALSE(GISLibrary::gmlPositionBounds("<D/>"));
}

// The KOR S-124 (no DatasetIdentificationInformation; the product from its namespace) and S-128
// (with one) are datasets of the System Database like the cells and grids.
TEST(SdbReaders, InspectsKoreanS124AndS128Gml)
{
	const fs::path s124 = testpaths::korS124Gml();
	const fs::path s128 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-128" / L"S-128" / L"128AA0020260522150149.gml";
	if (!fs::exists(s124) || !fs::exists(s128))
	{
		GTEST_SKIP() << "KOR S-124/S-128 test data not found under " << testpaths::tdsRoot();
	}
	const GISLibrary::GmlDatasetInspector inspector;
	ASSERT_TRUE(inspector.accepts(s124));
	EXPECT_FALSE(inspector.accepts(testpaths::korS101Cell()));
	EXPECT_FALSE(GISLibrary::Hdf5DatasetInspector().accepts(s124));

	const auto warning = inspector.inspect(s124);
	ASSERT_TRUE(warning);
	EXPECT_EQ(warning->name, "124KR.KCG.XI.S100.TEST1");
	EXPECT_EQ(warning->product, "S-124");
	EXPECT_EQ(warning->format, s100::sdb::DatasetFormat::Gml);
	ASSERT_TRUE(warning->bounds);
	// Off Busan North Port.
	EXPECT_NEAR(warning->bounds->minX, 129.05, 0.01);
	EXPECT_NEAR(warning->bounds->minY, 35.11, 0.01);
	ASSERT_EQ(warning->coverages.size(), 1u);
	EXPECT_TRUE(inKorea(warning->coverages[0].rings[0]));

	const auto products = inspector.inspect(s128);
	ASSERT_TRUE(products);
	EXPECT_EQ(products->product, "S-128");
	EXPECT_EQ(products->productEdition, "2.0");
	EXPECT_EQ(products->issueDate, "2026-05-22");
	EXPECT_EQ(products->updateNumber, 0);
	EXPECT_FALSE(products->isUpdate);
	ASSERT_TRUE(products->bounds);
	EXPECT_GE(products->bounds->minX, 110);
	EXPECT_LE(products->bounds->maxX, 150);
	EXPECT_GE(products->bounds->minY, 20);
	EXPECT_LE(products->bounds->maxY, 50);
	std::cout << "[ gml ] S-128 bounds " << products->bounds->minX << " " << products->bounds->minY << " "
		<< products->bounds->maxX << " " << products->bounds->maxY << "\n";

	// XML that is not an S-100 dataset.
	EXPECT_FALSE(inspector.inspect(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml"));
}

// A folder of loose .gml files, as the KOR S-124 comes: each dataset once, none passed over.
TEST(SdbReaders, ImportsAFolderOfGmlDatasets)
{
	const fs::path folder = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-124";
	if (!fs::exists(folder))
	{
		GTEST_SKIP() << "KOR S-124 test data not found: " << folder;
	}
	const auto root = testpaths::tempDir() / L"SdbReaders" / L"GmlFolder";
	fs::remove_all(root);
	s100::sdb::ResourceStore store(root);
	auto database = s100::sdb::SystemDatabase::open(store.databaseFile());
	const GISLibrary::GmlDatasetInspector gml;
	const GISLibrary::XmlCatalogueInspector catalogues;
	s100::sdb::Importer importer(database, store, s100::sdb::Importer::Readers{ { &gml }, &catalogues, nullptr });

	const auto report = importer.importFolder(folder);
	std::size_t added = 0;
	for (const auto& result : report.results)
	{
		std::cout << "[ gml ] " << result.source.filename().string() << ": " << static_cast<int>(result.outcome) << " " << result.message << "\n";
		if (result.outcome == s100::sdb::ImportOutcome::Added) ++added;
		EXPECT_EQ(result.message.find("no DataCoverage"), std::string::npos) << result.source;
	}
	EXPECT_GE(added, 2u) << "TEST1 and TEST2";
	const auto record = database.datasets().find("S-124", "124KR.KCG.XI.S100.TEST1");
	ASSERT_TRUE(record);
	EXPECT_EQ(record->format, s100::sdb::DatasetFormat::Gml);
	ASSERT_TRUE(record->bounds);
	EXPECT_TRUE(store.exists(record->path));

	// TEST2 is one circle of 185.2 m around a point: its bounds reach the radius.
	const auto circle = database.datasets().find("S-124", "124KR.KCG.XI.S100.TEST2");
	ASSERT_TRUE(circle && circle->bounds);
	EXPECT_NEAR(circle->bounds->maxY - circle->bounds->minY, 2 * 185.2 / 111320.0, 1e-6);
	EXPECT_NEAR((circle->bounds->minX + circle->bounds->maxX) / 2, 129.144397, 1e-6);
}
