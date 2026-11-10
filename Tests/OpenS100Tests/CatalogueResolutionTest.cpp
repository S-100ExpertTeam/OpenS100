#include <gtest/gtest.h>
#include "TestPaths.h"
#include "CatalogManager.h"
#include "SystemDatabase/CatalogueResolution.h"

#include <algorithm>

// System Database work item SDB-4: which installed catalogues portray a dataset. The cases of the
// first three suites are those of opens100-web packages/sdk/test/catalogue-registry.test.ts, with
// the same expected results.
using namespace s100::sdb;

namespace
{
	CatalogueRecord entry(std::string product, std::string version)
	{
		return CatalogueRecord{ .product = std::move(product), .version = std::move(version) };
	}

	std::string describe(const CatalogueResolution& resolution)
	{
		switch (resolution.match)
		{
		case CatalogueMatch::Ready: return "ready " + resolution.catalogue->version;
		case CatalogueMatch::Fallback:
			return std::string(resolution.reason == FallbackReason::Version ? "fallback(version) " : "fallback(unknown-version) ")
				+ resolution.catalogue->version;
		case CatalogueMatch::Unresolved: break;
		}
		std::string text = "unresolved [";
		for (const auto& version : resolution.available)
		{
			text += version + " ";
		}
		return text + "]";
	}
}

TEST(CatalogueVersions, ReadsTwoPointZeroAndTwoPointZeroPointZeroAsOneVersion)
{
	EXPECT_EQ(parseVersion("2.0"), (std::vector<int>{ 2, 0, 0 }));
	EXPECT_EQ(compareVersions("2.0", "2.0.0"), 0);
	EXPECT_FALSE(parseVersion("edition 2"));
	EXPECT_FALSE(parseVersion(""));
}

TEST(CatalogueVersions, OrdersByNumberNotByText)
{
	std::vector<std::string> versions = { "1.10.0", "2.0.0", "1.9.0" };
	std::ranges::sort(versions, [](const std::string& a, const std::string& b) { return compareVersions(a, b) < 0; });
	EXPECT_EQ(versions, (std::vector<std::string>{ "1.9.0", "1.10.0", "2.0.0" }));
	EXPECT_TRUE(sameMajorVersion("2.0.0", "2.1"));
	EXPECT_FALSE(sameMajorVersion("1.5.0", "2.0.0"));
}

TEST(CatalogueVersions, FindsTheProductHoweverItIsNamed)
{
	EXPECT_EQ(productFromIdentifier("S-101"), "S-101");
	EXPECT_EQ(productFromIdentifier("S101"), "S-101");
	EXPECT_EQ(productFromIdentifier("124"), "S-124");
	EXPECT_EQ(productFromIdentifier("INT.IHO.S-102.3.0.0"), "S-102");
	EXPECT_EQ(productFromIdentifier("ENC"), "");
	EXPECT_EQ(productFromIdentifier("S-1011"), "");
	EXPECT_EQ(productFromIdentifier("DATASET_FILES"), "");
}

TEST(CatalogueResolution, FollowsTheWebRegistryRules)
{
	const std::vector<CatalogueRecord> entries = { entry("S-101", "1.0.0"), entry("S-101", "2.0.0"), entry("S-124", "2.0.0") };
	auto withPatch = entries;
	withPatch.push_back(entry("S-101", "2.0.5"));

	EXPECT_EQ(describe(resolveCatalogue("S-101", "1.0", entries)), "ready 1.0.0");
	EXPECT_EQ(describe(resolveCatalogue("101", "2.1.0", withPatch)), "fallback(version) 2.0.5");
	EXPECT_EQ(describe(resolveCatalogue("S-101", "", entries)), "fallback(unknown-version) 2.0.0");
	EXPECT_EQ(describe(resolveCatalogue("S-101", "3.0.0", entries)), "unresolved [2.0.0 1.0.0 ]");
	EXPECT_EQ(describe(resolveCatalogue("S-102", "3.0.0", entries)), "unresolved []");
	// The KOR cells say "2.0"; the catalogues are issued as "2.0.0".
	EXPECT_EQ(describe(resolveCatalogue("INT.IHO.S-101.2.0", "2.0", entries)), "ready 2.0.0");
}

// --- CatalogManager: registered catalogues are read on first use -------------------------------

class CatalogManagerRegistry : public ::testing::Test
{
protected:
	void SetUp() override
	{
		fcFile = testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml";
		pcFile = testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml";
		manager.onPortrayalLoaded = [this](PortrayalCatalogue*) { ++portrayalLoads; };
		manager.registerFeatureCatalogue(entry("S-101", "2.0.0"), fcFile);
		manager.registerPortrayalCatalogue(entry("S-101", "2.0.0"), pcFile);
	}

	CatalogManager manager;
	std::filesystem::path fcFile;
	std::filesystem::path pcFile;
	int portrayalLoads = 0;
};

TEST_F(CatalogManagerRegistry, RegisteringReadsNothing)
{
	EXPECT_TRUE(manager.FCs.empty());
	EXPECT_TRUE(manager.PCs.empty());
	EXPECT_EQ(manager.registeredFeatureCatalogues().size(), 1u);
	EXPECT_EQ(portrayalLoads, 0);
}

TEST_F(CatalogManagerRegistry, ResolvingLoadsOnceAndReportsTheMatch)
{
	const auto first = manager.resolve("S-101", "2.0");
	ASSERT_NE(first.fc, nullptr);
	ASSERT_NE(first.pc, nullptr);
	EXPECT_EQ(first.feature.match, CatalogueMatch::Ready);
	EXPECT_EQ(first.fc->getProductId(), "S-101");
	EXPECT_EQ(portrayalLoads, 1);

	const auto again = manager.resolve("S-101", "2.0.0");
	EXPECT_EQ(again.fc, first.fc);
	EXPECT_EQ(again.pc, first.pc);
	EXPECT_EQ(manager.FCs.size(), 1u);
	EXPECT_EQ(portrayalLoads, 1);

	const auto newer = manager.resolve("S-101", "2.1");
	EXPECT_EQ(newer.feature.match, CatalogueMatch::Fallback);
	EXPECT_EQ(newer.fc, first.fc);

	const auto other = manager.resolve("S-124", "1.0");
	EXPECT_EQ(other.feature.match, CatalogueMatch::Unresolved);
	EXPECT_EQ(other.fc, nullptr);
}

TEST_F(CatalogManagerRegistry, GetByProductLoadsTheNewestRegistered)
{
	FeatureCatalogue* fc = manager.getFC(101);
	ASSERT_NE(fc, nullptr);
	EXPECT_EQ(manager.getFC("S-101"), fc);
	EXPECT_NE(manager.getPC("S-101"), nullptr);
	EXPECT_EQ(portrayalLoads, 1);
}

TEST_F(CatalogManagerRegistry, AFileThatFailsIsUnresolvedAndNotRetried)
{
	manager.registerFeatureCatalogue(entry("S-101", "2.0.0"), testpaths::tempDir() / "missing_FC.xml");
	EXPECT_EQ(manager.registeredFeatureCatalogues().size(), 1u);   // replaced, not added

	const auto resolution = manager.resolve("S-101", "2.0.0");
	EXPECT_EQ(resolution.fc, nullptr);
	EXPECT_EQ(resolution.feature.match, CatalogueMatch::Unresolved);
	EXPECT_EQ(manager.getFC("S-101"), nullptr);
}
