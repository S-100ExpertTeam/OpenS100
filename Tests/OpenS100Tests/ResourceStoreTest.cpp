#include <gtest/gtest.h>
#include "TestPaths.h"
#include "SystemDatabase/ResourceStore.h"
#include "SystemDatabase/Sha256.h"
#include "SystemDatabase/Utf8Path.h"

#include <filesystem>
#include <fstream>
#include <string>

// System Database work item SDB-2: the managed store's layout, copies, hashes and removal.
using namespace s100::sdb;

namespace
{
	std::filesystem::path freshDirectory(const std::filesystem::path& leaf)
	{
		auto dir = testpaths::tempDir() / L"ResourceStore" / leaf;
		std::filesystem::remove_all(dir);
		std::filesystem::create_directories(dir);
		return dir;
	}

	void writeText(const std::filesystem::path& file, const std::string& text)
	{
		std::filesystem::create_directories(file.parent_path());
		std::ofstream out(file, std::ios::binary | std::ios::trunc);
		out << text;
	}

	std::string readText(const std::filesystem::path& file)
	{
		std::ifstream in(file, std::ios::binary);
		return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}

	std::string hashOf(std::string_view text)
	{
		return sha256Hex(std::span(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
	}
}

TEST(Sha256, MatchesFips180Vectors)
{
	EXPECT_EQ(hashOf(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
	EXPECT_EQ(hashOf("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
	EXPECT_EQ(hashOf("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
		"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
}

TEST(Sha256, ChunkedInputHashesLikeOneBlock)
{
	std::string text;
	for (int i = 0; i < 1000; ++i)
	{
		text += std::to_string(i) + ",";
	}
	const auto* bytes = reinterpret_cast<const std::uint8_t*>(text.data());

	Sha256 chunked;
	for (std::size_t offset = 0; offset < text.size(); offset += 37)
	{
		chunked.update(std::span(bytes + offset, std::min<std::size_t>(37, text.size() - offset)));
	}
	EXPECT_EQ(Sha256::toHex(chunked.finish()), hashOf(text));
}

TEST(ResourceStore, LaysOutResourcesBySegment)
{
	EXPECT_EQ(ResourceStore::featureCataloguePath("S-101", "2.0.0", "S-101_FC.xml"), "FC/S-101/2.0.0/S-101_FC.xml");
	EXPECT_EQ(ResourceStore::portrayalCatalogueDirectory("S-101", "2.0.0"), "PC/S-101/2.0.0");
	EXPECT_EQ(ResourceStore::datasetFilePath("S-101", "101KR003G2M00", "101KR003G2M00.000"),
		"DATASET/S-101/101KR003G2M00/101KR003G2M00.000");
	EXPECT_EQ(ResourceStore::supportFilePath("S-101", "101KR000010E100.TXT"), "SUPPORT/S-101/101KR000010E100.TXT");
}

TEST(ResourceStore, SegmentsCannotLeaveTheirPlace)
{
	EXPECT_EQ(ResourceStore::safeSegment("../x"), ".._x");
	EXPECT_EQ(ResourceStore::safeSegment(".."), "._");   // trailing dots are replaced, so no ".." survives
	EXPECT_EQ(ResourceStore::safeSegment(""), "_");
	EXPECT_EQ(ResourceStore::safeSegment("a\\b:c"), "a_b_c");

	ResourceStore store(freshDirectory(L"escape"));
	EXPECT_THROW((void)store.absolute("../outside"), std::invalid_argument);
	EXPECT_THROW((void)store.absolute("FC/../../outside"), std::invalid_argument);
	EXPECT_THROW((void)store.absolute("C:/absolute"), std::invalid_argument);
}

TEST(ResourceStore, CopiesFileAndReturnsItsHash)
{
	// Non-ASCII root: the store must hand the file system UTF-16, not the ANSI code page.
	const auto dir = freshDirectory(L"\uC800\uC7A5\uC18C");
	const auto source = dir / L"source" / L"\uC140.000";
	writeText(source, "first edition");
	ResourceStore store(dir / L"root");

	const std::string relative = ResourceStore::datasetFilePath("S-101", "\uC140", "\uC140.000");
	const std::string digest = store.copyFile(source, relative);

	EXPECT_EQ(digest, hashOf("first edition"));
	EXPECT_TRUE(store.exists(relative));
	EXPECT_EQ(readText(store.absolute(relative)), "first edition");
	EXPECT_FALSE(std::filesystem::exists(store.absolute(relative + ".part")));

	// Copying again replaces the file.
	writeText(source, "re-issue");
	EXPECT_EQ(store.copyFile(source, relative), hashOf("re-issue"));
	EXPECT_EQ(readText(store.absolute(relative)), "re-issue");
}

TEST(ResourceStore, CopiesDirectoryTreeReplacingWhatWasThere)
{
	const auto dir = freshDirectory(L"tree");
	writeText(dir / "pc" / "portrayal_catalogue.xml", "<pc/>");
	writeText(dir / "pc" / "Rules" / "main.lua", "-- rules");
	ResourceStore store(dir / "root");
	const std::string target = ResourceStore::portrayalCatalogueDirectory("S-101", "2.0.0");
	writeText(store.absolute(target + "/stale.txt"), "old");

	const auto copied = store.copyDirectory(dir / "pc", target);

	ASSERT_EQ(copied.size(), 2u);
	EXPECT_EQ(copied[0], "PC/S-101/2.0.0/Rules/main.lua");
	EXPECT_EQ(copied[1], "PC/S-101/2.0.0/portrayal_catalogue.xml");
	EXPECT_FALSE(store.exists(target + "/stale.txt"));
	EXPECT_EQ(readText(store.absolute(target + "/Rules/main.lua")), "-- rules");
}

TEST(ResourceStore, RemovePrunesEmptyDirectoriesButKeepsTheRoot)
{
	const auto dir = freshDirectory(L"remove");
	writeText(dir / "a.000", "a");
	ResourceStore store(dir / "root");
	const std::string first = ResourceStore::datasetFilePath("S-101", "A", "A.000");
	const std::string second = ResourceStore::datasetFilePath("S-101", "B", "B.000");
	(void)store.copyFile(dir / "a.000", first);
	(void)store.copyFile(dir / "a.000", second);

	EXPECT_TRUE(store.remove(ResourceStore::datasetDirectory("S-101", "A")));
	EXPECT_FALSE(std::filesystem::exists(store.absolute("DATASET/S-101/A")));
	EXPECT_TRUE(std::filesystem::exists(store.absolute("DATASET/S-101")));   // B is still there

	EXPECT_TRUE(store.remove(second));
	EXPECT_FALSE(std::filesystem::exists(store.absolute("DATASET")));
	EXPECT_TRUE(std::filesystem::exists(store.root()));
	EXPECT_FALSE(store.remove(second));
}

TEST(ResourceStore, MoveReplacesTheDestination)
{
	const auto dir = freshDirectory(L"move");
	writeText(dir / "x", "new");
	ResourceStore store(dir / "root");
	(void)store.copyFile(dir / "x", "STAGING/1/x");
	writeText(store.absolute("SUPPORT/S-101/x"), "old");

	store.move("STAGING/1/x", "SUPPORT/S-101/x");

	EXPECT_EQ(readText(store.absolute("SUPPORT/S-101/x")), "new");
	EXPECT_FALSE(std::filesystem::exists(store.absolute("STAGING")));
}
