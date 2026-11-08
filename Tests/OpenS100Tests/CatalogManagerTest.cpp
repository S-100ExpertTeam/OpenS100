#include <gtest/gtest.h>
#include <pugixml.hpp>
#include "TestPaths.h"
#include "CatalogManager.h"

// Phase 1 step 2-3: catalogue management needs no renderer. Render caches for a
// portrayal catalogue are built separately by GISLibrary::PortrayalRender::Prepare.
TEST(CatalogManager, LoadsCataloguesWithoutRenderer)
{
	CatalogManager cm;

	auto fc = cm.addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string());
	ASSERT_NE(fc, nullptr);
	EXPECT_EQ(cm.getFC("S-101"), fc);
	EXPECT_EQ(cm.getFC(101), fc);
	EXPECT_EQ(cm.getPC("S-101"), nullptr) << "no portrayal catalogue added yet";

	auto pc = cm.addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
	ASSERT_NE(pc, nullptr);
	EXPECT_EQ(cm.getPC("S-101"), pc);
}

TEST(CatalogManager, UnknownProductIsNull)
{
	CatalogManager cm;
	EXPECT_EQ(cm.getFC("S-999"), nullptr);
	EXPECT_EQ(cm.getPC("S-999"), nullptr);
}
