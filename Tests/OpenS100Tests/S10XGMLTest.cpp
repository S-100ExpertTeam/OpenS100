#include <afxwin.h>
#include <gtest/gtest.h>
#include <pugixml.hpp>
#include "TestPaths.h"
#include "FeatureCatalogue.h"
#include "S10XGML.h"

// Phase 1 step 1-6: a GML dataset opens with a directly bound feature catalogue,
// no Layer and no renderer.
TEST(S10XGML, OpensKoreanS124WithoutRenderer)
{
	auto gmlPath = testpaths::korS124Gml();
	if (!std::filesystem::exists(gmlPath))
	{
		GTEST_SKIP() << "reference GML not found: " << gmlPath;
	}
	FeatureCatalogue fc;
	ASSERT_TRUE(fc.Read((testpaths::repoRoot() / "ProgramData" / "FC" / "S-124_FC.xml").string()));

	S10XGML gml;
	gml.SetFC(&fc);
	ASSERT_TRUE(gml.Open(gmlPath.wstring()));
	EXPECT_GT(gml.features.size(), 0u);
	EXPECT_GT(gml.geometries.size(), 0u);
}

TEST(S10XGML, MissingFileReturnsFalse)
{
	S10XGML gml;
	EXPECT_FALSE(gml.Open(std::wstring(L"Z:\does\not\exist.gml")));
}
