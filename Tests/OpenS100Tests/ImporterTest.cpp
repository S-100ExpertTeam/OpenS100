#include <gtest/gtest.h>
#include "TestPaths.h"
#include "SystemDatabase/Geometry.h"
#include "SystemDatabase/Importer.h"
#include "SystemDatabase/ResourceStore.h"
#include "SystemDatabase/SystemDatabase.h"

#include <filesystem>
#include <fstream>
#include <map>

// System Database work item SDB-3: the importer's edition/update rules, its file handling and
// exchange set import, with stand-in readers (the real ISO 8211 reader is tested with the data in
// SdbInspectorTest.cpp).
using namespace s100::sdb;
namespace fs = std::filesystem;

namespace
{
	fs::path freshDirectory(const fs::path& leaf)
	{
		auto dir = testpaths::tempDir() / L"Importer" / leaf;
		fs::remove_all(dir);
		fs::create_directories(dir);
		return dir;
	}

	void writeText(const fs::path& file, const std::string& text)
	{
		fs::create_directories(file.parent_path());
		std::ofstream out(file, std::ios::binary | std::ios::trunc);
		out << text;
	}

	Ring square(double x0, double y0, double x1, double y1)
	{
		return { { x0, y0 }, { x1, y0 }, { x1, y1 }, { x0, y1 } };
	}

	InspectedDataset cell(std::string name, int edition, int update = 0, bool isUpdate = false)
	{
		return InspectedDataset{
			.name = std::move(name),
			.product = "S-101",
			.productEdition = "2.0.0",
			.format = DatasetFormat::Iso8211,
			.edition = edition,
			.isUpdate = isUpdate,
			.updateNumber = update,
			.issueDate = "2026-07-03",
			.coverages = { InspectedCoverage{ .optimumDisplayScale = 22000, .minimumDisplayScale = 90000,
				.rings = { square(126, 34, 127, 35) } } },
		};
	}

	// Answers for the files the test has registered, by path.
	class FakeDatasetInspector final : public DatasetInspector
	{
	public:
		std::map<fs::path, InspectedDataset> answers;

		bool accepts(const fs::path& file) const override { return file.extension() != ".txt"; }

		std::optional<InspectedDataset> inspect(const fs::path& file) const override
		{
			const auto found = answers.find(file);
			return found == answers.end() ? std::nullopt : std::optional(found->second);
		}
	};

	class FakeCatalogueInspector final : public CatalogueInspector
	{
	public:
		std::map<fs::path, InspectedCatalogue> answers;

		std::optional<InspectedCatalogue> inspectFeatureCatalogue(const fs::path& xml) const override { return find(xml); }
		std::optional<InspectedCatalogue> inspectPortrayalCatalogue(const fs::path& xml) const override { return find(xml); }

	private:
		std::optional<InspectedCatalogue> find(const fs::path& xml) const
		{
			const auto found = answers.find(xml);
			return found == answers.end() ? std::nullopt : std::optional(found->second);
		}
	};

	class FakeExchangeSetReader final : public ExchangeSetReader
	{
	public:
		std::optional<InspectedExchangeSet> answer;

		std::optional<InspectedExchangeSet> read(const fs::path&) const override { return answer; }
	};

	class ImporterTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
			dir = freshDirectory(info->name());
			store = std::make_unique<ResourceStore>(dir / "root");
			importer = std::make_unique<Importer>(database, *store,
				Importer::Readers{ { &datasets }, &catalogues, &exchangeSets },
				[] { return std::string("2026-11-10T00:00:00Z"); });
		}

		// A source file with distinct content, registered with the fake dataset reader.
		fs::path source(const std::string& relative, const InspectedDataset& answer, const std::string& content = {})
		{
			const fs::path file = dir / "source" / fs::path(relative);
			writeText(file, content.empty() ? relative : content);
			datasets.answers[file] = answer;
			return file;
		}

		fs::path dir;
		SystemDatabase database = SystemDatabase::openInMemory();
		std::unique_ptr<ResourceStore> store;
		FakeDatasetInspector datasets;
		FakeCatalogueInspector catalogues;
		FakeExchangeSetReader exchangeSets;
		std::unique_ptr<Importer> importer;
	};
}

// --- rules -----------------------------------------------------------------------------------

TEST(DatasetDecision, FollowsEditionAndUpdateRules)
{
	DatasetRecord installed{ .name = "A", .product = "S-101", .edition = 2, .updateNumber = 0 };
	const auto decide = [&](const InspectedDataset& incoming, int highest, bool same = false) {
		return decideDataset(incoming, installed, highest, same).action;
	};

	EXPECT_EQ(decideDataset(cell("A", 1), std::nullopt, 0, false).action, DatasetAction::Add);
	EXPECT_EQ(decideDataset(cell("A", 1, 1, true), std::nullopt, 0, false).action, DatasetAction::Reject);
	EXPECT_EQ(decide(cell("A", 3), 0), DatasetAction::Replace);                // new edition
	EXPECT_EQ(decide(cell("A", 1), 0), DatasetAction::Reject);                 // older edition
	EXPECT_EQ(decide(cell("A", 2), 0, true), DatasetAction::Unchanged);        // the same file again
	EXPECT_EQ(decide(cell("A", 2), 0), DatasetAction::Replace);                // re-issue
	EXPECT_EQ(decide(cell("A", 2), 3, true), DatasetAction::Unchanged);        // its base again, updates installed
	EXPECT_EQ(decide(cell("A", 2, 1), 3), DatasetAction::Reject);              // re-issue older than the updates
	EXPECT_EQ(decide(cell("A", 2, 3), 3), DatasetAction::Replace);             // re-issue including them
	EXPECT_EQ(decide(cell("A", 2, 1, true), 0), DatasetAction::AddUpdate);
	EXPECT_EQ(decide(cell("A", 2, 1, true), 1), DatasetAction::Unchanged);
	EXPECT_EQ(decide(cell("A", 2, 3, true), 1), DatasetAction::Reject);        // update 2 missing
	EXPECT_EQ(decide(cell("A", 1, 1, true), 0), DatasetAction::Reject);        // for another edition
	EXPECT_EQ(decide(cell("A", 0, 2, true), 1), DatasetAction::Cancel);

	installed.status = DatasetStatus::Cancelled;
	EXPECT_EQ(decide(cell("A", 0, 3, true), 2), DatasetAction::Unchanged);
	EXPECT_EQ(decide(cell("A", 2, 3, true), 2), DatasetAction::Reject);
	EXPECT_EQ(decide(cell("A", 2), 2), DatasetAction::Reject);
	EXPECT_EQ(decide(cell("A", 3), 2), DatasetAction::Replace);                // a new edition reinstates it
}

// --- datasets --------------------------------------------------------------------------------

TEST_F(ImporterTest, InstallsDatasetWithCoverageAndCopy)
{
	const auto file = source("A/A.000", cell("A", 1));

	const auto imported = importer->importDataset(file);

	ASSERT_EQ(imported.outcome, ImportOutcome::Added) << imported.message;
	const auto record = database.datasets().find(imported.id);
	ASSERT_TRUE(record);
	EXPECT_EQ(record->path, "DATASET/S-101/A/A.000");
	EXPECT_EQ(record->edition, 1);
	ASSERT_TRUE(record->bounds);
	EXPECT_EQ(*record->bounds, (BoundingBox{ 126, 34, 127, 35 }));
	EXPECT_TRUE(store->exists(record->path));
	EXPECT_FALSE(fs::exists(store->absolute("STAGING")));

	const auto coverages = database.coverages().forDataset(imported.id);
	ASSERT_EQ(coverages.size(), 1u);
	EXPECT_EQ(coverages[0].optimumDisplayScale, 22000);
	const auto rings = decodeWkbPolygon(coverages[0].polygon);
	ASSERT_TRUE(rings);
	ASSERT_EQ(rings->size(), 1u);
	EXPECT_EQ((*rings)[0].size(), 5u);   // closed on encoding

	EXPECT_EQ(importer->importDataset(file).outcome, ImportOutcome::Unchanged);
}

TEST_F(ImporterTest, RecordsUpdatesAndDropsThemForANewEdition)
{
	const auto base = importer->importDataset(source("A/A.000", cell("A", 1)));
	ASSERT_EQ(base.outcome, ImportOutcome::Added);

	EXPECT_EQ(importer->importDataset(source("A/A.002", cell("A", 1, 2, true))).outcome, ImportOutcome::Rejected);
	const auto update = importer->importDataset(source("A/A.001", cell("A", 1, 1, true)));
	ASSERT_EQ(update.outcome, ImportOutcome::UpdateAdded) << update.message;
	ASSERT_EQ(database.datasets().updates(base.id).size(), 1u);
	EXPECT_TRUE(store->exists("DATASET/S-101/A/A.001"));

	const auto edition2 = importer->importDataset(source("A2/A.000", cell("A", 2), "edition 2"));
	ASSERT_EQ(edition2.outcome, ImportOutcome::Replaced) << edition2.message;
	EXPECT_EQ(edition2.id, base.id);
	EXPECT_TRUE(database.datasets().updates(base.id).empty());
	EXPECT_FALSE(store->exists("DATASET/S-101/A/A.001"));
	EXPECT_TRUE(store->exists("DATASET/S-101/A/A.000"));
	EXPECT_EQ(database.datasets().find(base.id)->edition, 2);
}

TEST_F(ImporterTest, CancellationRemovesTheDatasetFromSelection)
{
	const auto base = importer->importDataset(source("A/A.000", cell("A", 1)));
	ASSERT_EQ(database.coverages().active().size(), 1u);

	const auto cancelled = importer->importDataset(source("A/A.001", cell("A", 0, 1, true)));

	EXPECT_EQ(cancelled.outcome, ImportOutcome::Cancelled);
	EXPECT_EQ(database.datasets().find(base.id)->status, DatasetStatus::Cancelled);
	EXPECT_TRUE(database.coverages().active().empty());
}

TEST_F(ImporterTest, RejectsWhatItCannotRead)
{
	const fs::path unknown = dir / "source" / "B.000";
	writeText(unknown, "not registered");
	EXPECT_EQ(importer->importDataset(unknown).outcome, ImportOutcome::Rejected);

	InspectedDataset nameless = cell("C", 1);
	nameless.product.clear();
	EXPECT_EQ(importer->importDataset(source("C/C.000", nameless)).outcome, ImportOutcome::Rejected);
	EXPECT_TRUE(database.datasets().all().empty());
}

TEST_F(ImporterTest, RemovingADatasetDeletesItsFiles)
{
	const auto base = importer->importDataset(source("A/A.000", cell("A", 1)));
	ASSERT_EQ(importer->importDataset(source("A/A.001", cell("A", 1, 1, true))).outcome, ImportOutcome::UpdateAdded);

	EXPECT_TRUE(importer->removeDataset(base.id));

	EXPECT_FALSE(database.datasets().find(base.id));
	EXPECT_FALSE(fs::exists(store->absolute("DATASET")));
	EXPECT_FALSE(importer->removeDataset(base.id));
}

// --- catalogues and support files ------------------------------------------------------------

TEST_F(ImporterTest, InstallsFeatureCatalogueByProductAndVersion)
{
	const fs::path xml = dir / "source" / "S-101_FC.xml";
	writeText(xml, "<fc v1/>");
	catalogues.answers[xml] = InspectedCatalogue{ "S-101", "2.0.0", "2024-09-23" };

	const auto added = importer->importFeatureCatalogue(xml);
	ASSERT_EQ(added.outcome, ImportOutcome::Added) << added.message;
	EXPECT_EQ(database.featureCatalogues().find("S-101", "2.0.0")->path, "FC/S-101/2.0.0/S-101_FC.xml");
	EXPECT_EQ(importer->importFeatureCatalogue(xml).outcome, ImportOutcome::Unchanged);

	writeText(xml, "<fc v1 corrected/>");
	EXPECT_EQ(importer->importFeatureCatalogue(xml).outcome, ImportOutcome::Replaced);
	EXPECT_EQ(database.featureCatalogues().all().size(), 1u);

	EXPECT_TRUE(importer->removeFeatureCatalogue(added.id));
	EXPECT_FALSE(fs::exists(store->absolute("FC")));
}

TEST_F(ImporterTest, InstallsPortrayalCatalogueFolder)
{
	const fs::path folder = dir / "source" / "PC";
	writeText(folder / "portrayal_catalogue.xml", "<pc/>");
	writeText(folder / "Rules" / "main.lua", "-- v1");
	catalogues.answers[folder / "portrayal_catalogue.xml"] = InspectedCatalogue{ "S-101", "2.0.0", "" };

	const auto added = importer->importPortrayalCatalogue(folder);
	ASSERT_EQ(added.outcome, ImportOutcome::Added) << added.message;
	EXPECT_EQ(database.portrayalCatalogues().find(added.id)->path, "PC/S-101/2.0.0/portrayal_catalogue.xml");
	EXPECT_TRUE(store->exists("PC/S-101/2.0.0/Rules/main.lua"));
	EXPECT_EQ(importer->importPortrayalCatalogue(folder / "portrayal_catalogue.xml").outcome, ImportOutcome::Unchanged);

	// A changed rule file changes the catalogue, though portrayal_catalogue.xml did not change.
	writeText(folder / "Rules" / "main.lua", "-- v2");
	EXPECT_EQ(importer->importPortrayalCatalogue(folder).outcome, ImportOutcome::Replaced);

	EXPECT_TRUE(importer->removePortrayalCatalogue(added.id));
	EXPECT_FALSE(fs::exists(store->absolute("PC")));
}

TEST_F(ImporterTest, SupportFilesAreReplacedAndDeletedByPurpose)
{
	const fs::path text = dir / "source" / "101KR000010E100.TXT";
	writeText(text, "notes");

	const auto added = importer->importSupportFile(text, "S-101", "new");
	ASSERT_EQ(added.outcome, ImportOutcome::Added);
	EXPECT_EQ(database.supportFiles().find(added.id)->path, "SUPPORT/S-101/101KR000010E100.TXT");

	writeText(text, "corrected notes");
	EXPECT_EQ(importer->importSupportFile(text, "S-101", "replacement").outcome, ImportOutcome::Replaced);
	EXPECT_EQ(importer->importSupportFile(text, "S-101", "deletion").outcome, ImportOutcome::Removed);
	EXPECT_TRUE(database.supportFiles().all().empty());
	EXPECT_FALSE(store->exists("SUPPORT/S-101/101KR000010E100.TXT"));
}

// --- exchange sets ---------------------------------------------------------------------------

TEST_F(ImporterTest, ExchangeSetInstallsEverythingItListsAndLinksSupportFiles)
{
	const fs::path text = dir / "source" / "S-101" / "SUPPORT_FILES" / "NOTE.TXT";
	writeText(text, "notes");
	InspectedDataset a = cell("A", 1);
	a.supportFileReferences = { "NOTE.TXT" };
	InspectedDataset b = cell("B", 1);
	b.coverages.clear();   // the catalogue's coverage stands in
	const fs::path fileA = source("S-101/A/A.000", a);
	const fs::path fileB = source("S-101/B/B.000", b);
	const fs::path updateA = source("S-101/A/A.001", cell("A", 1, 1, true));

	InspectedExchangeSet set;
	set.identifier = "KR00-test";
	set.issueDate = "2026-11-06T08:27:15Z";
	// The update is listed first; the importer installs bases before their updates.
	set.datasets.push_back({ .file = updateA });
	set.datasets.push_back({ .file = fileA });
	set.datasets.push_back({ .file = fileB, .coverages = { InspectedCoverage{ .optimumDisplayScale = 45000,
		.rings = { square(10, 10, 11, 11) } } } });
	set.datasets.push_back({ .file = dir / "source" / "missing.000" });
	set.supportFiles.push_back({ .file = text, .product = "S-101", .purpose = "new" });
	exchangeSets.answer = set;

	std::size_t progressCalls = 0;
	const auto report = importer->importExchangeSet(dir / "source" / "CATALOG.XML",
		[&](std::size_t done, std::size_t total) { ++progressCalls; EXPECT_LE(done, total); });

	ASSERT_TRUE(report.exchangeSetId);
	EXPECT_EQ(progressCalls, 4u);
	EXPECT_EQ(report.count(ImportOutcome::Added), 3u);         // A, B and the support file
	EXPECT_EQ(report.count(ImportOutcome::UpdateAdded), 1u);
	EXPECT_EQ(report.count(ImportOutcome::Rejected), 1u);      // the missing file, without stopping the rest
	EXPECT_TRUE(report.installProblems.empty());

	const auto installedA = database.datasets().find("S-101", "A");
	const auto installedB = database.datasets().find("S-101", "B");
	ASSERT_TRUE(installedA && installedB);
	EXPECT_EQ(installedA->exchangeSetId, report.exchangeSetId);
	EXPECT_EQ(database.datasets().supportFileIds(installedA->id).size(), 1u);
	ASSERT_EQ(database.coverages().forDataset(installedB->id).size(), 1u);
	EXPECT_EQ(database.coverages().forDataset(installedB->id)[0].optimumDisplayScale, 45000);
	EXPECT_EQ(database.exchangeSets().find(*report.exchangeSetId)->identifier, "KR00-test");

	// Importing it again changes nothing but records the exchange set once more.
	const auto again = importer->importExchangeSet(dir / "source" / "CATALOG.XML");
	EXPECT_EQ(again.count(ImportOutcome::Unchanged), 4u);
	EXPECT_EQ(database.datasets().all().size(), 2u);
}

TEST_F(ImporterTest, UnreadableCatalogueImportsTheFilesInItsFolder)
{
	const fs::path catalogue = dir / "source" / "CATALOG.XML";
	writeText(catalogue, "<not a catalogue/>");
	const fs::path text = dir / "source" / "S-101" / "SUPPORT_FILES" / "note.txt";
	writeText(text, "notes");
	source("S-101/DATASET_FILES/A/A.000", cell("A", 1));
	exchangeSets.answer.reset();   // the reader cannot read it

	const auto report = importer->importExchangeSet(catalogue);

	EXPECT_FALSE(report.exchangeSetId);
	EXPECT_EQ(report.count(ImportOutcome::Rejected), 1u);   // the catalogue itself, saying what was done
	EXPECT_EQ(report.count(ImportOutcome::Added), 2u);      // the dataset and the support file
	EXPECT_TRUE(database.datasets().find("S-101", "A"));
	EXPECT_EQ(database.supportFiles().all().size(), 1u);
}

// --- folders ---------------------------------------------------------------------------------

TEST_F(ImporterTest, FolderImportsTheSameFileOnce)
{
	source("copy1/S-101/DATASET_FILES/A/A.000", cell("A", 1), "cell A");
	source("copy2/S-101/DATASET_FILES/A/A.000", cell("A", 1), "cell A");   // same name, same bytes
	source("copy2/S-101/DATASET_FILES/B/B.000", cell("B", 1), "cell B");
	writeText(dir / "source" / "copy1" / "S-101" / "SUPPORT_FILES" / "note.txt", "notes");
	writeText(dir / "source" / "copy2" / "S-101" / "SUPPORT_FILES" / "note.txt", "notes");
	writeText(dir / "source" / "copy2" / "S-101" / "SUPPORT_FILES" / "other.txt", "notes");   // same bytes, another name
	writeText(dir / "source" / "readme.txt", "not a support file");   // outside SUPPORT_FILES: left out

	const auto report = importer->importFolder(dir / "source");

	EXPECT_EQ(report.count(ImportOutcome::Added), 4u);       // A, B, note.txt, other.txt
	EXPECT_EQ(report.count(ImportOutcome::Unchanged), 2u);   // the second A and note.txt
	EXPECT_EQ(report.count(ImportOutcome::Rejected), 0u);
	EXPECT_EQ(database.datasets().all().size(), 2u);
	EXPECT_EQ(database.supportFiles().all().size(), 2u);
	const auto copy = std::ranges::find(report.results, ImportOutcome::Unchanged, &ImportResult::outcome);
	ASSERT_NE(copy, report.results.end());
	EXPECT_NE(copy->message.find("same file as"), std::string::npos);
}

TEST_F(ImporterTest, FolderImportsACopiedExchangeSetOnce)
{
	const fs::path fileA = source("set1/S-101/DATASET_FILES/A/A.000", cell("A", 1), "cell A");
	source("set2/S-101/DATASET_FILES/A/A.000", cell("A", 1), "cell A");
	writeText(dir / "source" / "set1" / "CATALOG.XML", "<catalogue/>");
	writeText(dir / "source" / "set2" / "CATALOG.XML", "<catalogue/>");
	InspectedExchangeSet set;
	set.identifier = "KR00-test";
	set.datasets.push_back({ .file = fileA });
	exchangeSets.answer = set;

	const auto report = importer->importFolder(dir / "source");

	ASSERT_TRUE(report.exchangeSetId);
	EXPECT_EQ(report.count(ImportOutcome::Added), 1u);
	EXPECT_EQ(report.count(ImportOutcome::Unchanged), 1u);   // set2's catalogue: its files are not looked at
	EXPECT_EQ(database.exchangeSets().all().size(), 1u);
	EXPECT_EQ(database.datasets().all().size(), 1u);
}

// --- geometry --------------------------------------------------------------------------------

TEST(Wkb, PolygonRoundTripsAndClosesRings)
{
	const PolygonRings rings = { square(0, 0, 10, 10), square(2, 2, 4, 4) };
	const auto wkb = encodeWkbPolygon(rings);
	EXPECT_EQ(wkb.size(), 1u + 4 + 4 + 2 * (4 + 5 * 16));

	const auto decoded = decodeWkbPolygon(wkb);
	ASSERT_TRUE(decoded);
	ASSERT_EQ(decoded->size(), 2u);
	EXPECT_EQ((*decoded)[1].front(), (Position{ 2, 2 }));
	EXPECT_EQ((*decoded)[1].back(), (Position{ 2, 2 }));
}

TEST(Wkb, ReadsMultiPolygonAndRefusesTruncatedInput)
{
	const auto one = encodeWkbPolygon(PolygonRings{ square(0, 0, 1, 1) });
	std::vector<std::uint8_t> multi = { 1, 6, 0, 0, 0, 2, 0, 0, 0 };
	multi.insert(multi.end(), one.begin(), one.end());
	multi.insert(multi.end(), one.begin(), one.end());
	const auto decoded = decodeWkbPolygon(multi);
	ASSERT_TRUE(decoded);
	EXPECT_EQ(decoded->size(), 2u);

	auto truncated = one;
	truncated.resize(truncated.size() - 3);
	EXPECT_FALSE(decodeWkbPolygon(truncated));
	EXPECT_FALSE(decodeWkbPolygon(std::vector<std::uint8_t>{ 1, 1, 0, 0, 0 }));   // a point
}
