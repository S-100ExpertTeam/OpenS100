#include <gtest/gtest.h>
#include <pugixml.hpp>
#include <fstream>
#include <vector>
#include "TestPaths.h"
#include "FeatureCatalogue.h"
#include "S101Cell.h"

namespace
{
	std::vector<char> readAll(const std::filesystem::path& p)
	{
		std::ifstream in(p, std::ios::binary);
		return std::vector<char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}

	void writeAll(const std::filesystem::path& p, const std::vector<char>& bytes)
	{
		std::ofstream out(p, std::ios::binary | std::ios::trunc);
		out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
	}

	// ISO 8211 leaders are 24 bytes: [0,5) record length, [12,17) base address of field area.
	int leaderInt(const std::vector<char>& b, size_t at, size_t len)
	{
		return std::atoi(std::string(b.begin() + at, b.begin() + at + len).c_str());
	}
}

class S101CellKorTds : public ::testing::Test
{
protected:
	void SetUp() override
	{
		cellPath = testpaths::korS101Cell();
		if (!std::filesystem::exists(cellPath))
		{
			GTEST_SKIP() << "reference cell not found: " << cellPath;
		}
		auto fcPath = testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml";
		ASSERT_TRUE(fc.Read(fcPath.string()));
	}

	std::filesystem::path cellPath;
	FeatureCatalogue fc;
};

TEST_F(S101CellKorTds, ParsesSmallestKoreanCell)
{
	S101Cell cell(&fc);
	ASSERT_TRUE(cell.Read8211(cellPath.wstring()));

	EXPECT_GT(cell.GetCount_FeatureTypeRecord(), 0);
	const int spatial = cell.GetCount_PointRecord() + cell.GetCount_MultiPointRecord()
		+ cell.GetCount_CurveRecord() + cell.GetCount_CompositeCurveRecord() + cell.GetCount_SurfaceRecord();
	EXPECT_GT(spatial, 0);
}

// Regression for the record loop in S101Cell::Read8211 / OpenMetadataBy000:
// a data record whose leader declares zero length and a field-area offset that
// yields no directory entries used to spin forever. The reader must return.
// Phase 1 step 1: opening a dataset must not require a renderer. Direct2D geometry
// is created lazily at draw time, so Open() with D2D1Resources == nullptr succeeds.
TEST_F(S101CellKorTds, OpensSmallestKoreanCellWithoutRenderer)
{
	S101Cell cell(&fc);
	ASSERT_TRUE(cell.Open(cellPath.wstring()));

	EXPECT_GT(cell.GetCount_FeatureTypeRecord(), 0);
	EXPECT_GT(cell.GetFeatureCount(), 0);
	ASSERT_GT(cell.GetCount_CurveRecord(), 0);
}

// The ISO 8211 writer now streams through s100::io::BinaryWriter. Writing a cell and
// reading it back must preserve the record counts.
TEST_F(S101CellKorTds, SaveAsEncRoundTrip)
{
	S101Cell cell(&fc);
	ASSERT_TRUE(cell.Open(cellPath.wstring()));

	auto out = testpaths::tempDir() / "roundtrip.000";
	ASSERT_TRUE(cell.SaveAsENC(out.wstring()));
	ASSERT_GT(std::filesystem::file_size(out), 0u);

	S101Cell again(&fc);
	ASSERT_TRUE(again.Read8211(out.wstring()));
	EXPECT_EQ(again.GetCount_FeatureTypeRecord(), cell.GetCount_FeatureTypeRecord());
	EXPECT_EQ(again.GetCount_InformationRecord(), cell.GetCount_InformationRecord());
	EXPECT_EQ(again.GetCount_PointRecord(), cell.GetCount_PointRecord());
	EXPECT_EQ(again.GetCount_CurveRecord(), cell.GetCount_CurveRecord());
	EXPECT_EQ(again.GetCount_SurfaceRecord(), cell.GetCount_SurfaceRecord());
}

TEST_F(S101CellKorTds, MalformedRecordLeaderDoesNotHang)
{
	auto bytes = readAll(cellPath);
	ASSERT_GT(bytes.size(), 48u);

	const int ddrLength = leaderInt(bytes, 0, 5);
	ASSERT_GT(ddrLength, 24);
	ASSERT_LT(static_cast<size_t>(ddrLength) + 24, bytes.size());

	// First data record: record length 0, field area at 0 -> subFieldCount < 1.
	const std::string zeros = "00000";
	std::copy(zeros.begin(), zeros.end(), bytes.begin() + ddrLength);
	std::copy(zeros.begin(), zeros.end(), bytes.begin() + ddrLength + 12);

	auto corrupt = testpaths::tempDir() / "malformed_leader.000";
	writeAll(corrupt, bytes);

	S101Cell cell(&fc);
	(void)cell.Open(corrupt.wstring());   // must return; result is unspecified
	SUCCEED();
}

TEST_F(S101CellKorTds, RecordLengthBeyondBufferDoesNotCrash)
{
	auto bytes = readAll(cellPath);
	const int ddrLength = leaderInt(bytes, 0, 5);
	ASSERT_LT(static_cast<size_t>(ddrLength) + 24, bytes.size());

	// First data record claims to be 99998 bytes long, far past the end of the file.
	const std::string huge = "99998";
	std::copy(huge.begin(), huge.end(), bytes.begin() + ddrLength);

	auto corrupt = testpaths::tempDir() / "oversized_record.000";
	writeAll(corrupt, bytes);

	S101Cell cell(&fc);
	(void)cell.Open(corrupt.wstring());
	SUCCEED();
}

TEST_F(S101CellKorTds, TruncatedFileDoesNotCrash)
{
	auto bytes = readAll(cellPath);
	bytes.resize(bytes.size() / 2);

	auto corrupt = testpaths::tempDir() / "truncated.000";
	writeAll(corrupt, bytes);

	S101Cell cell(&fc);
	(void)cell.Open(corrupt.wstring());
	SUCCEED();
}

TEST(S101Cell, MissingFileReturnsFalse)
{
	FeatureCatalogue fc;
	S101Cell cell(&fc);
	EXPECT_FALSE(cell.Read8211(L"Z:\does\not\exist\101XX000.000"));
	EXPECT_FALSE(cell.Open(std::wstring(L"Z:\does\not\exist\101XX000.000")));
}

TEST(S101Cell, ReadTwiceIntoSameCellIsStable)
{
	auto cellPath = testpaths::korS101Cell();
	if (!std::filesystem::exists(cellPath))
	{
		GTEST_SKIP() << "reference cell not found: " << cellPath;
	}
	FeatureCatalogue fc;
	ASSERT_TRUE(fc.Read((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()));

	// Read8211() calls RemoveAll() first and the destructor calls it again; the
	// record maps and updateInformation must survive that sequence (double delete fix).
	S101Cell cell(&fc);
	ASSERT_TRUE(cell.Read8211(cellPath.wstring()));
	const int first = cell.GetCount_FeatureTypeRecord();
	ASSERT_TRUE(cell.Read8211(cellPath.wstring()));
	EXPECT_EQ(first, cell.GetCount_FeatureTypeRecord());
}
