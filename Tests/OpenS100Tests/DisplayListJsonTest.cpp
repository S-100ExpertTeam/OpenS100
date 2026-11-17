// The display list as text (GISLibrary::DisplayListToJson).
//
// The golden test portrays the S-101 cell that ships in SampleData with the catalogues that ship
// in ProgramData and compares the hash of the text. The same hash is what the consumer example
// prints on every platform, so a change here is a change of what the library says is to be
// drawn: either the rules or catalogues changed, or the portrayal code did.
#include <gtest/gtest.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <string>

#include "TestPaths.h"
#include "DisplayListJson.h"
#include "FeatureCatalogue.h"
#include "PCOutputSchemaManager.h"
#include "S100Layer.h"
#include "S101Cell.h"
#include "S10XGML.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaInstruction.h"
#include "SENC_DisplayList.h"
#include "SENC_LineInstruction.h"
#include "SENC_LineStyle.h"
#include "SENC_PointInstruction.h"
#include "SENC_Symbol.h"
#include "SENC_Dash.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

namespace
{
	// Portrays one dataset the way a consumer of the package does: no renderer, no window.
	// An S-101 cell (ISO 8211) runs the Lua rules, a GML dataset the XSLT rules.
	std::string PortrayToJson(const char* fcFile, const char* pcFolder, const std::filesystem::path& datasetPath)
	{
		const auto root = testpaths::repoRoot();
		FeatureCatalogue fc;
		EXPECT_TRUE(fc.Read((root / "ProgramData" / "FC" / fcFile).string()));
		PortrayalCatalogue pc;
		EXPECT_TRUE(pc.Open((root / "ProgramData" / "PC" / pcFolder / "portrayal_catalogue.xml").string()));

		S100Layer layer(&fc, &pc);
		S100SpatialObject* dataset = nullptr;   // the layer owns its dataset
		if (datasetPath.extension() == ".gml")
		{
			auto* gml = new S10XGML();
			gml->SetFC(&fc);
			dataset = gml;
		}
		else
		{
			dataset = new S101Cell(&fc);
		}
		layer.m_spatialObject = dataset;
		dataset->SetLayer(&layer);
		if (!dataset->Open(datasetPath.wstring()))
		{
			ADD_FAILURE() << "cannot read " << datasetPath;
			return {};
		}
		layer.BuildPortrayalCatalogue();
		if (!dataset->pcManager || !dataset->pcManager->displayListSENC)
		{
			ADD_FAILURE() << "no display list";
			return {};
		}
		return GISLibrary::DisplayListToJson(*dataset->pcManager->displayListSENC);
	}

	std::string PortrayS101ToJson(const std::filesystem::path& cellPath)
	{
		return PortrayToJson("S-101_FC.xml", "S101_Portrayal", cellPath);
	}

	// The hash of the text; on a mismatch the text is left in the temp directory of the run.
	void ExpectGoldenHash(const std::string& json, const std::string& golden, const char* dumpName)
	{
		const std::string hash = std::format("{:016x}", GISLibrary::JsonHash(json));
		if (hash != golden)
		{
			const auto dump = testpaths::tempDir() / dumpName;
			std::ofstream(dump, std::ios::binary | std::ios::trunc) << json;
			ADD_FAILURE() << "display list hash " << hash << ", " << json.size() << " bytes; the text is in " << dump;
		}
	}
}

TEST(DisplayListJson, EmptyListIsAnEmptyArray)
{
	SENC_DisplayList list;
	EXPECT_EQ(GISLibrary::DisplayListToJson(list), "{\"instructions\":[\n]}\n");
}

TEST(DisplayListJson, PointInstructionHasTheFieldsOfTheWebDisplayList)
{
	SENC_PointInstruction point;
	point.featureReference = "12";
	point.viewingGroup = { 27010, 27011 };
	point.displayPlane = 1;
	point.drawingPriority = 24;
	point.scaleMinimum = 0;
	point.scaleMaximum = 45000;
	point.symbol = new SENC_Symbol();   // the instruction owns it
	point.symbol->reference = L"BOYLAT13";
	point.symbol->rotation = 22.5;
	point.symbol->scaleFactor = 1;
	point.symbol->offset.x = 0.1;

	EXPECT_EQ(GISLibrary::InstructionToJson(point),
		"{\"type\":\"point\",\"featureReference\":\"12\",\"spatialReferences\":[],"
		"\"viewingGroups\":[27010,27011],\"displayPlane\":\"OverRadar\",\"drawingPriority\":24,"
		"\"scaleMinimum\":null,\"scaleMaximum\":45000,"
		"\"symbol\":{\"reference\":\"BOYLAT13\",\"rotation\":22.5,\"rotationCRS\":\"PortrayalCRS\","
		"\"scaleFactor\":1,\"offset\":{\"x\":0.1,\"y\":0}}}");
}

TEST(DisplayListJson, LineStyleWritesDashesAndASolidLineHasNoInterval)
{
	SENC_LineInstruction line;
	line.featureReference = "7";
	line.displayPlane = 0;
	line.drawingPriority = 15;
	line.scaleMinimum = 0;
	line.scaleMaximum = 0;
	auto* style = new SENC_LineStyle();   // the instruction owns it
	style->pen_width = 0.32;
	style->colorToken = L"CHMGD";
	style->intervalLength = 5.4;
	auto* dash = new SENC_Dash();         // the style owns it
	dash->start = 0;
	dash->length = 3.6f;
	style->dashes.push_back(dash);
	line.lineStyles.push_back(style);
	line.lineStyles.push_back(new SENC_LineStyle());

	const std::string json = GISLibrary::InstructionToJson(line);
	EXPECT_NE(json.find("\"width\":0.32,\"color\":{\"token\":\"CHMGD\",\"transparency\":0},\"intervalLength\":5.4,"
		"\"dashes\":[{\"start\":0,\"length\":3.6}]"), std::string::npos) << json;
	EXPECT_NE(json.find("\"intervalLength\":null,\"dashes\":[]"), std::string::npos) << json;
}

TEST(DisplayListJson, TextIsEscapedAndWrittenAsUtf8)
{
	SENC_AreaInstruction area;
	area.featureReference = "a\"b\\c\n";
	area.displayPlane = 0;
	area.drawingPriority = 3;
	area.scaleMinimum = 0;
	area.scaleMaximum = 0;
	auto* fill = new SENC_ColorFill();    // the instruction owns it
	fill->color = new SENC_Color();       // the fill owns it
	fill->color->token = L"DEPDW";
	fill->color->transparency = 0.25f;
	area.areaFill = fill;

	const std::string json = GISLibrary::InstructionToJson(area);
	EXPECT_NE(json.find("\"featureReference\":\"a\\\"b\\\\c\\n\""), std::string::npos) << json;
	EXPECT_NE(json.find("\"fill\":{\"type\":\"colorFill\",\"color\":{\"token\":\"DEPDW\",\"transparency\":0.25}}"),
		std::string::npos) << json;
}

TEST(DisplayListJson, SampleCellMatchesTheGoldenHash)
{
	const auto cellPath = testpaths::repoRoot() / "SampleData" / "10100AA_X01SE.000";
	ASSERT_TRUE(std::filesystem::exists(cellPath)) << cellPath;

	const std::string json = PortrayS101ToJson(cellPath);
	ASSERT_FALSE(json.empty());

	// Twice gives the same text: nothing in it depends on addresses or on what was left in memory.
	EXPECT_EQ(PortrayS101ToJson(cellPath), json);

	// The same value the consumer example prints, on Windows and on Linux.
	ExpectGoldenHash(json, "74cada0555afe32c", "10100AA_X01SE.json");
}

// The other engine: a GML dataset portrayed by the XSLT rules of its catalogue.
TEST(DisplayListJson, XsltSampleMatchesTheGoldenHash)
{
	const auto gmlPath = testpaths::repoRoot() / "SampleData" / "12900MCTDS120TS.gml";
	ASSERT_TRUE(std::filesystem::exists(gmlPath)) << gmlPath;

	const std::string json = PortrayToJson("S-129_FC.xml", "S129_Portrayal", gmlPath);
	ASSERT_FALSE(json.empty());
	EXPECT_NE(json.find("\"type\":"), std::string::npos) << "no instruction: " << json;
	EXPECT_EQ(PortrayToJson("S-129_FC.xml", "S129_Portrayal", gmlPath), json);

	ExpectGoldenHash(json, "544183a60186ab0d", "12900MCTDS120TS.json");
}
