#include <gtest/gtest.h>
#include <pugixml.hpp>
#include "TestPaths.h"
#include "FeatureCatalogue.h"

TEST(FeatureCatalogue, ReadsBundledS101Catalogue)
{
	auto path = testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml";
	ASSERT_TRUE(std::filesystem::exists(path)) << path;

	FeatureCatalogue fc;
	EXPECT_TRUE(fc.Read(path.string()));
}

TEST(FeatureCatalogue, ReadReturnsFalseForMissingFile)
{
	FeatureCatalogue fc;
	EXPECT_FALSE(fc.Read(std::string("Z:\\does\\not\\exist\\FC.xml")));
}
