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

// Signed exchange sets carry dataStatus="unencrypted" (S-100 Part 15 spelling) on every entry.
TEST(ExchangeCatalogue, OpensSignedCatalogueWithLowerCaseDataStatus)
{
	const auto path = testpaths::tempDir() / L"SignedCatalogue.xml";
	{
		pugi::xml_document doc;
		ASSERT_TRUE(doc.load_string(R"(<?xml version="1.0" encoding="UTF-8"?>
<S100XC:S100_ExchangeCatalogue xmlns:S100XC="http://www.iho.int/s100/xc/5.2" xmlns:S100SE="http://www.iho.int/s100/se/5.2">
  <S100XC:identifier><S100XC:identifier>KR00-test</S100XC:identifier><S100XC:dateTime>2026-11-06T06:16:21Z</S100XC:dateTime></S100XC:identifier>
  <S100XC:certificates><S100SE:schemeAdministrator id="IHO"/><S100SE:certificate id="urn:mrn:iho:KR01:00281" issuer="IHO">AAAA</S100SE:certificate></S100XC:certificates>
  <S100XC:datasetDiscoveryMetadata>
    <S100XC:S100_DatasetDiscoveryMetadata>
      <S100XC:fileName>file:/S-101/DATASET_FILES/101KR00TEST/101KR00TEST.000</S100XC:fileName>
      <S100XC:digitalSignatureReference>ECDSA-384-SHA2</S100XC:digitalSignatureReference>
      <S100XC:digitalSignatureValue>
        <S100SE:S100_SE_SignatureOnData dataStatus="unencrypted" id="S-101/DATASET_FILES/101KR00TEST/101KR00TEST.000" certificateRef="urn:mrn:iho:KR01:00281">AAAA</S100SE:S100_SE_SignatureOnData>
      </S100XC:digitalSignatureValue>
    </S100XC:S100_DatasetDiscoveryMetadata>
  </S100XC:datasetDiscoveryMetadata>
</S100XC:S100_ExchangeCatalogue>)"));
		ASSERT_TRUE(doc.save_file(path.c_str()));
	}

	S100::ExchangeCatalogue ec;
	ASSERT_NO_THROW(EXPECT_TRUE(ec.OpenFile(path)));
	ASSERT_EQ(ec.DatasetDiscoveryMetadata.size(), 1u);
	EXPECT_EQ(ec.DatasetDiscoveryMetadata[0].FileName, "file:/S-101/DATASET_FILES/101KR00TEST/101KR00TEST.000");
}

TEST(ExchangeCatalogue, MissingFileLeavesCatalogueEmpty)
{
	S100::ExchangeCatalogue ec;
	ec.Open("Z:\\does\\not\\exist\\CATALOG.XML");
	EXPECT_EQ(ec.Identifier, nullptr);
}
