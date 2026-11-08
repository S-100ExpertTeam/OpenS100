#include <afxwin.h>
#include <gtest/gtest.h>
#include <pugixml.hpp>
#include "TestPaths.h"
#include "ExchangeCatalogue.h"

TEST(ExchangeCatalogue, OpensKoreanS101ExchangeSetCatalogue)
{
	auto path = testpaths::korS101Catalog();
	if (!std::filesystem::exists(path))
	{
		GTEST_SKIP() << "CATALOG.XML not found: " << path;
	}

	S100::ExchangeCatalogue ec;
	ec.Open(path.string());
	EXPECT_NE(ec.Identifier, nullptr) << "exchange catalogue identifier was not parsed";
}

TEST(ExchangeCatalogue, MissingFileLeavesCatalogueEmpty)
{
	S100::ExchangeCatalogue ec;
	ec.Open("Z:\\does\\not\\exist\\CATALOG.XML");
	EXPECT_EQ(ec.Identifier, nullptr);
}
