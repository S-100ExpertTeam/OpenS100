#include <gtest/gtest.h>
#include <sqlite3.h>
#include "TestPaths.h"
#include "SystemDatabase/SystemDatabase.h"
#include "SystemDatabase/Schema.h"

#include <array>
#include <filesystem>

// System Database (Documents/TechDocs/2026-11-10-system-database.md), work item SDB-1:
// schema, migration, transactions and the repositories.
using namespace s100::sdb;

namespace
{
	CatalogueRecord s101Catalogue(std::string version)
	{
		return CatalogueRecord{
			.product = "S-101",
			.version = version,
			.path = "FC/S-101/" + version + "/S-101_FC.xml",
			.sha256 = "00",
			.issueDate = "2024-06-01",
			.importedAt = "2026-11-10T00:00:00Z",
		};
	}

	DatasetRecord s101Dataset(std::string name)
	{
		return DatasetRecord{
			.name = name,
			.product = "S-101",
			.productEdition = "2.0.0",
			.format = DatasetFormat::Iso8211,
			.edition = 1,
			.updateNumber = 0,
			.issueDate = "2025-01-01",
			.path = "DATASET/S-101/" + name + "/" + name + ".000",
			.bounds = BoundingBox{ 126.0, 34.0, 127.0, 35.0 },
			.status = DatasetStatus::Active,
			.exchangeSetId = std::nullopt,
			.importedAt = "2026-11-10T00:00:00Z",
		};
	}

	DataCoverageRecord coverage(int optimum, int minimum)
	{
		return DataCoverageRecord{
			.optimumDisplayScale = optimum,
			.minimumDisplayScale = minimum,
			.maximumDisplayScale = std::nullopt,
			.bounds = BoundingBox{ 126.0, 34.0, 127.0, 35.0 },
			.polygon = { 0x01, 0x03, 0x00, 0x00, 0x00 },
		};
	}

	// A fresh directory under the test scratch area, removed again by the fixture.
	std::filesystem::path freshDirectory(const std::filesystem::path& leaf)
	{
		auto dir = testpaths::tempDir() / L"SystemDatabase" / leaf;
		std::filesystem::remove_all(dir);
		return dir;
	}
}

TEST(SystemDatabase, InMemoryDatabaseHasCurrentSchema)
{
	auto db = SystemDatabase::openInMemory();
	EXPECT_EQ(db.schemaVersion(), schema::CurrentVersion);
	EXPECT_TRUE(db.datasets().all().empty());
	EXPECT_TRUE(db.featureCatalogues().all().empty());
}

TEST(SystemDatabase, FileDatabasePersistsAcrossReopen)
{
	// Non-ASCII directory: the path must reach SQLite as UTF-8, not the ANSI code page.
	const auto dir = freshDirectory(L"\uC2DC\uC2A4\uD15C DB");
	const auto file = dir / L"sdb.sqlite";
	{
		auto db = SystemDatabase::open(file);
		db.featureCatalogues().add(s101Catalogue("2.0.0"));
	}
	ASSERT_TRUE(std::filesystem::exists(file));
	{
		auto db = SystemDatabase::open(file);
		EXPECT_EQ(db.schemaVersion(), schema::CurrentVersion);
		auto fc = db.featureCatalogues().find("S-101", "2.0.0");
		ASSERT_TRUE(fc.has_value());
		EXPECT_EQ(fc->path, "FC/S-101/2.0.0/S-101_FC.xml");
	}
	std::filesystem::remove_all(dir);
}

TEST(SystemDatabase, RefusesNewerSchema)
{
	const auto dir = freshDirectory(L"newer");
	const auto file = dir / L"sdb.sqlite";
	(void)SystemDatabase::open(file);
	{
		sqlite3* raw = nullptr;
		const auto name = file.u8string();
		ASSERT_EQ(sqlite3_open(std::string(name.begin(), name.end()).c_str(), &raw), SQLITE_OK);
		ASSERT_EQ(sqlite3_exec(raw, "PRAGMA user_version = 99", nullptr, nullptr, nullptr), SQLITE_OK);
		sqlite3_close(raw);
	}
	EXPECT_THROW((void)SystemDatabase::open(file), DatabaseError);
	std::filesystem::remove_all(dir);
}

TEST(SystemDatabase, MoveKeepsRepositoriesWorking)
{
	auto first = SystemDatabase::openInMemory();
	first.datasets().add(s101Dataset("101KR003G2M00"));
	SystemDatabase moved = std::move(first);
	EXPECT_EQ(moved.datasets().all().size(), 1u);
}

TEST(SystemDatabase, CatalogueRepositoryRoundTrip)
{
	auto db = SystemDatabase::openInMemory();
	auto& fcs = db.featureCatalogues();

	const auto id200 = fcs.add(s101Catalogue("2.0.0"));
	fcs.add(s101Catalogue("1.0.0"));
	EXPECT_GT(id200, 0);

	auto all = fcs.all();
	ASSERT_EQ(all.size(), 2u);
	EXPECT_EQ(all[0].version, "1.0.0") << "ordered by product, then version";

	auto stored = fcs.find(id200);
	ASSERT_TRUE(stored.has_value());
	auto expected = s101Catalogue("2.0.0");
	expected.id = id200;
	EXPECT_EQ(*stored, expected);

	stored->sha256 = "ff";
	EXPECT_TRUE(fcs.update(*stored));
	EXPECT_EQ(fcs.find(id200)->sha256, "ff");

	EXPECT_TRUE(fcs.remove(id200));
	EXPECT_FALSE(fcs.remove(id200));
	EXPECT_FALSE(fcs.find("S-101", "2.0.0").has_value());
	EXPECT_EQ(fcs.forProduct("S-101").size(), 1u);
}

TEST(SystemDatabase, FeatureAndPortrayalCataloguesAreSeparate)
{
	auto db = SystemDatabase::openInMemory();
	db.featureCatalogues().add(s101Catalogue("2.0.0"));
	EXPECT_TRUE(db.portrayalCatalogues().all().empty());

	auto pc = s101Catalogue("2.0.0");
	pc.path = "PC/S-101/2.0.0/portrayal_catalogue.xml";
	db.portrayalCatalogues().add(pc);
	EXPECT_EQ(db.portrayalCatalogues().find("S-101", "2.0.0")->path, pc.path);
}

TEST(SystemDatabase, DuplicateCatalogueIsAConstraintError)
{
	auto db = SystemDatabase::openInMemory();
	db.featureCatalogues().add(s101Catalogue("2.0.0"));
	auto again = s101Catalogue("2.0.0");
	again.path = "FC/elsewhere.xml";
	try
	{
		db.featureCatalogues().add(again);
		FAIL() << "expected DatabaseError";
	}
	catch (const DatabaseError& error)
	{
		EXPECT_EQ(error.code(), SQLITE_CONSTRAINT_UNIQUE);
	}
}

TEST(SystemDatabase, DatasetRepositoryRoundTrip)
{
	auto db = SystemDatabase::openInMemory();
	auto& datasets = db.datasets();

	auto record = s101Dataset("101KR003G2M00");
	record.id = datasets.add(record);

	auto stored = datasets.find("S-101", "101KR003G2M00");
	ASSERT_TRUE(stored.has_value());
	EXPECT_EQ(*stored, record);

	stored->bounds.reset();
	stored->edition.reset();
	stored->status = DatasetStatus::Cancelled;
	stored->format = DatasetFormat::Gml;
	EXPECT_TRUE(datasets.update(*stored));
	EXPECT_EQ(*datasets.find(record.id), *stored);

	EXPECT_FALSE(datasets.find("S-101", "missing").has_value());
}

TEST(SystemDatabase, DatasetUpdatesAreOrderedAndUnique)
{
	auto db = SystemDatabase::openInMemory();
	const auto id = db.datasets().add(s101Dataset("101KR003G2M00"));
	auto& datasets = db.datasets();

	datasets.addUpdate({ .datasetId = id, .updateNumber = 2, .path = "DATASET/S-101/101KR003G2M00/101KR003G2M00.002" });
	datasets.addUpdate({ .datasetId = id, .updateNumber = 1, .path = "DATASET/S-101/101KR003G2M00/101KR003G2M00.001" });
	EXPECT_THROW(datasets.addUpdate({ .datasetId = id, .updateNumber = 1, .path = "other.001" }), DatabaseError);

	EXPECT_TRUE(datasets.markUpdateApplied(id, 1));
	EXPECT_FALSE(datasets.markUpdateApplied(id, 7));

	auto updates = datasets.updates(id);
	ASSERT_EQ(updates.size(), 2u);
	EXPECT_EQ(updates[0].updateNumber, 1);
	EXPECT_TRUE(updates[0].applied);
	EXPECT_FALSE(updates[1].applied);

	EXPECT_EQ(datasets.removeUpdates(id), 2);
	EXPECT_TRUE(datasets.updates(id).empty());
}

TEST(SystemDatabase, UpdateForUnknownDatasetIsRejected)
{
	auto db = SystemDatabase::openInMemory();
	EXPECT_THROW(db.datasets().addUpdate({ .datasetId = 42, .updateNumber = 1, .path = "x.001" }), DatabaseError)
		<< "foreign keys must be enforced";
}

TEST(SystemDatabase, CoveragesReplaceAndFilterCancelledDatasets)
{
	auto db = SystemDatabase::openInMemory();
	const auto active = db.datasets().add(s101Dataset("101KR003G2M00"));
	auto cancelledRecord = s101Dataset("101KR004G2M00");
	cancelledRecord.status = DatasetStatus::Cancelled;
	const auto cancelled = db.datasets().add(cancelledRecord);

	const std::array first = { coverage(22000, 45000) };
	db.coverages().replace(active, first);
	const std::array second = { coverage(12000, 22000), coverage(8000, 12000) };
	const auto ids = db.coverages().replace(active, second);
	ASSERT_EQ(ids.size(), 2u);
	db.coverages().replace(cancelled, first);

	auto stored = db.coverages().forDataset(active);
	ASSERT_EQ(stored.size(), 2u) << "replace drops the previous coverages";
	EXPECT_EQ(stored[0].id, ids[0]);
	EXPECT_EQ(stored[0].datasetId, active);
	EXPECT_EQ(stored[0].optimumDisplayScale, 12000);
	EXPECT_FALSE(stored[0].maximumDisplayScale.has_value());
	EXPECT_EQ(stored[0].polygon, second[0].polygon);
	EXPECT_EQ(stored[1].bounds, second[1].bounds);

	auto activeCoverages = db.coverages().active();
	ASSERT_EQ(activeCoverages.size(), 2u) << "coverages of cancelled datasets are not selectable";
	EXPECT_EQ(activeCoverages[0].datasetId, active);
}

TEST(SystemDatabase, RemovingDatasetCascades)
{
	auto db = SystemDatabase::openInMemory();
	const auto dataset = db.datasets().add(s101Dataset("101KR003G2M00"));
	const auto support = db.supportFiles().add({
		.name = "KR_PIC01.jpg", .path = "SUPPORT/S-101/KR_PIC01.jpg", .purpose = "new",
		.sha256 = "00", .importedAt = "2026-11-10T00:00:00Z" });

	db.datasets().addUpdate({ .datasetId = dataset, .updateNumber = 1, .path = "u.001" });
	const std::array covers = { coverage(22000, 45000) };
	db.coverages().replace(dataset, covers);
	db.datasets().linkSupportFile(dataset, support);
	db.datasets().linkSupportFile(dataset, support);
	EXPECT_EQ(db.datasets().supportFileIds(dataset).size(), 1u) << "linking is idempotent";
	EXPECT_EQ(db.datasets().datasetsReferencing(support), std::vector<RecordId>{ dataset });

	EXPECT_TRUE(db.datasets().remove(dataset));
	EXPECT_TRUE(db.datasets().updates(dataset).empty());
	EXPECT_TRUE(db.coverages().forDataset(dataset).empty());
	EXPECT_TRUE(db.datasets().datasetsReferencing(support).empty());
	EXPECT_TRUE(db.supportFiles().find(support).has_value()) << "support files are shared and stay";
}

TEST(SystemDatabase, SupportFileRepositoryRoundTrip)
{
	auto db = SystemDatabase::openInMemory();
	SupportFileRecord record{
		.name = "KR_TXT01.TXT", .path = "SUPPORT/S-101/KR_TXT01.TXT", .purpose = "new", .edition = 1,
		.format = "TXT", .issueDate = "2025-01-01", .sha256 = "ab", .importedAt = "2026-11-10T00:00:00Z" };
	record.id = db.supportFiles().add(record);

	EXPECT_EQ(*db.supportFiles().findByPath(record.path), record);
	record.purpose = "replacement";
	EXPECT_TRUE(db.supportFiles().update(record));
	EXPECT_EQ(db.supportFiles().all().front().purpose, "replacement");
	EXPECT_TRUE(db.supportFiles().remove(record.id));
	EXPECT_TRUE(db.supportFiles().all().empty());
}

TEST(SystemDatabase, RemovingExchangeSetKeepsItsDatasets)
{
	auto db = SystemDatabase::openInMemory();
	const auto exchangeSet = db.exchangeSets().add({
		.sourcePath = "D:/KOR_TDS/S100_ROOT/CATALOG.XML", .identifier = "KR", .importedAt = "2026-11-10T00:00:00Z" });
	auto record = s101Dataset("101KR003G2M00");
	record.exchangeSetId = exchangeSet;
	const auto dataset = db.datasets().add(record);
	EXPECT_EQ(db.datasets().inExchangeSet(exchangeSet).size(), 1u);

	EXPECT_TRUE(db.exchangeSets().remove(exchangeSet));
	auto stored = db.datasets().find(dataset);
	ASSERT_TRUE(stored.has_value());
	EXPECT_FALSE(stored->exchangeSetId.has_value());
}

TEST(SystemDatabase, TransactionRollsBackUnlessCommitted)
{
	auto db = SystemDatabase::openInMemory();
	{
		auto transaction = db.transaction();
		db.datasets().add(s101Dataset("101KR003G2M00"));
	}
	EXPECT_TRUE(db.datasets().all().empty());
	{
		auto transaction = db.transaction();
		db.datasets().add(s101Dataset("101KR003G2M00"));
		db.datasets().add(s101Dataset("101KR004G2M00"));
		transaction.commit();
	}
	EXPECT_EQ(db.datasets().all().size(), 2u);
}

TEST(SystemDatabase, TextRoundTripsAsUtf8)
{
	auto db = SystemDatabase::openInMemory();
	auto record = s101Dataset("101KR003G2M00");
	record.path = "DATASET/S-101/\xED\x95\x9C\xEA\xB8\x80/101KR003G2M00.000";   // UTF-8 for two Hangul syllables
	db.datasets().add(record);
	EXPECT_EQ(db.datasets().all().front().path, record.path);
}
