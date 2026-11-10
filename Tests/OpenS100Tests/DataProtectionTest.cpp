#include <gtest/gtest.h>
#include "TestPaths.h"
#include "SdbReaders.h"
#include "SystemDatabase/DataProtection.h"
#include "SystemDatabase/Importer.h"
#include "SystemDatabase/ProtectedImport.h"
#include "SystemDatabase/ResourceStore.h"
#include "SystemDatabase/SystemDatabase.h"
#include "SystemDatabase/Utf8Path.h"
#include "SystemDatabase/ZipArchive.h"

#include <openssl/evp.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

// S-100 Part 15 data protection (DataProtection, ZipArchive, ProtectedImport). The vectors in the
// first tests were produced with the ECC no.ecc.s100.security library that secom-client uses
// (S100Manufacturer, S100DataPermit.create, S100Crypt.RandomIV over a java.util.zip archive), so
// they check this port against that implementation rather than against itself.
using namespace s100::sdb;
namespace fs = std::filesystem;

namespace
{
	// secom-client's sample manufacturer and system (application.properties, s100.p15.sample.*).
	constexpr std::string_view sampleMId = "859868";
	constexpr std::string_view sampleMKey = "4D5A79677065774A7343705272664F72";
	constexpr std::string_view sampleHwId = "40384B45B54596201114FE9904220101";
	constexpr std::string_view sampleUserPermit = "AD1DAD797C966EC9F6A55B66ED98281599B3C7B1859868";

	// Made with the ECC library: S100DataPermit.create("101KR00TEST.000", ..., dataKey, sampleHwId)
	// and S100Crypt.RandomIV(dataKey).encrypt(zip of javaPlainText() as "101KR00TEST.000").
	constexpr std::string_view javaDataKey = "0F1E2D3C4B5A69788796A5B4C3D2E1F0";
	constexpr std::string_view javaEncryptedKey = "B31FD6C4D9772D3813709943B4E5C7C9";
	constexpr std::string_view javaEncryptedFile =
		"46EFD7B282E46EC42CD677C3D1F4F91C98DBBD90E1E349E88A7E3FDB0A08BE5028393D5A290C1EF8DF11A1218EA5282F"
		"13EE29B1BE2AD8A5199E6405A7AA535328862545AF1E0E3D572C0CD567190CF8FEB3A972E0D3C475AE850078619F3B30"
		"7786377BCE8FF963506335C3756A109346D0E82789A007E8101EB5840A108A564C07DB847921430628A5A78D8183FDAA"
		"B3932B2BC1F110FF1A231301140E6F02EFB4E82CD98D90FDCADA6B2A682A7B8066424C692F9417382716BA374BB72CD1"
		"DC6F06742EF1D735E6EC8503372E01C91086836E2CC777D26EBAAE1C0CA1D2FB8F61F320E04F5E3F8EB4223F6B750600"
		"9DFF1DD7771884CD26397CDDD0E62C4FE7AE2C7790CFC5DB28BDE6CD38EDEC63C0161DC26F48817644090981FBAB5C6A"
		"62123486FF84F00CA159B579B5F0D735";

	std::string javaPlainText()
	{
		std::string text;
		for (int i = 0; i < 40; ++i)
		{
			text += std::format("S-100 Part 15 test line {}\n", i);
		}
		return text;
	}

	Bytes bytesOf(std::string_view text)
	{
		return Bytes(text.begin(), text.end());
	}

	std::string textOf(const Bytes& bytes)
	{
		return std::string(bytes.begin(), bytes.end());
	}

	Bytes readBytes(const fs::path& file)
	{
		std::ifstream in(file, std::ios::binary);
		return Bytes{ std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
	}

	void writeBytes(const fs::path& file, std::span<const std::uint8_t> bytes)
	{
		fs::create_directories(file.parent_path());
		std::ofstream out(file, std::ios::binary | std::ios::trunc);
		out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
	}

	void writeText(const fs::path& file, std::string_view text)
	{
		writeBytes(file, bytesOf(text));
	}

	// A ZIP archive of stored entries, as an exchange set might be delivered.
	Bytes storedZip(const std::vector<std::pair<std::string, Bytes>>& entries)
	{
		Bytes zip;
		Bytes directory;
		const auto put = [](Bytes& out, std::uint32_t value, int bytes) {
			for (int i = 0; i < bytes; ++i)
			{
				out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
			}
		};
		for (const auto& [name, content] : entries)
		{
			const auto offset = static_cast<std::uint32_t>(zip.size());
			const auto crc = computeCrc32(content);
			const auto size = static_cast<std::uint32_t>(content.size());
			const auto nameLength = static_cast<std::uint32_t>(name.size());
			put(zip, 0x04034b50, 4); put(zip, 20, 2); put(zip, 0, 2); put(zip, 0, 2); put(zip, 0, 4);
			put(zip, crc, 4); put(zip, size, 4); put(zip, size, 4); put(zip, nameLength, 2); put(zip, 0, 2);
			zip.insert(zip.end(), name.begin(), name.end());
			zip.insert(zip.end(), content.begin(), content.end());

			put(directory, 0x02014b50, 4); put(directory, 20, 2); put(directory, 20, 2); put(directory, 0, 2);
			put(directory, 0, 2); put(directory, 0, 4); put(directory, crc, 4); put(directory, size, 4);
			put(directory, size, 4); put(directory, nameLength, 2); put(directory, 0, 2); put(directory, 0, 2);
			put(directory, 0, 2); put(directory, 0, 2); put(directory, 0, 4); put(directory, offset, 4);
			directory.insert(directory.end(), name.begin(), name.end());
		}
		const auto directoryOffset = static_cast<std::uint32_t>(zip.size());
		zip.insert(zip.end(), directory.begin(), directory.end());
		const auto count = static_cast<std::uint32_t>(entries.size());
		put(zip, 0x06054b50, 4); put(zip, 0, 2); put(zip, 0, 2); put(zip, count, 2); put(zip, count, 2);
		put(zip, static_cast<std::uint32_t>(directory.size()), 4); put(zip, directoryOffset, 4); put(zip, 0, 2);
		return zip;
	}

	struct PermitEntry
	{
		std::string fileName;
		Bytes dataKey;
		std::string expiry = "2099-12-31";
	};

	// PERMIT.XML in the layout secom-client writes (s100_permit_template.xml).
	std::string permitXml(std::string_view userPermit, std::string_view hwId, const std::vector<PermitEntry>& entries)
	{
		std::string xml = std::format(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<s100:Permit xmlns:s100=\"http://www.iho.int/S100/2.0\">\n"
			"  <s100:header>\n"
			"    <s100:issueDate>2026-11-11</s100:issueDate>\n"
			"    <s100:dataServerName>KHOA</s100:dataServerName>\n"
			"    <s100:dataServerIdentifier>urn:mrn:iho:KR00:00280</s100:dataServerIdentifier>\n"
			"    <s100:version>1.0</s100:version>\n"
			"    <s100:userpermit>{}</s100:userpermit>\n"
			"  </s100:header>\n"
			"  <s100:products>\n"
			"    <s100:product id=\"S-101\">\n", userPermit);
		for (const auto& entry : entries)
		{
			xml += std::format(
				"      <s100:datasetPermit>\n"
				"        <s100:filename>{}</s100:filename>\n"
				"        <s100:editionNumber>1</s100:editionNumber>\n"
				"        <s100:expiry>{}</s100:expiry>\n"
				"        <s100:encryptedKey>{}</s100:encryptedKey>\n"
				"      </s100:datasetPermit>\n", entry.fileName, entry.expiry, encryptDataKey(entry.dataKey, hwId));
		}
		xml += "    </s100:product>\n  </s100:products>\n</s100:Permit>\n";
		return xml;
	}
}

// --- the user permit -------------------------------------------------------------------------

TEST(DataProtection, ParsesTheSampleUserPermit)
{
	const auto permit = UserPermit::parse(sampleUserPermit);
	EXPECT_EQ(permit.encryptedHwId, "AD1DAD797C966EC9F6A55B66ED982815");
	EXPECT_EQ(permit.checksum, "99B3C7B1");
	EXPECT_EQ(permit.manufacturerId, sampleMId);
	EXPECT_EQ(permit.toString(), sampleUserPermit);
}

TEST(DataProtection, DecryptsTheHwIdWithTheManufacturerKey)
{
	EXPECT_EQ(decryptHwId(UserPermit::parse(sampleUserPermit), sampleMKey), sampleHwId);
	// Lower-case input is the same key.
	EXPECT_EQ(decryptHwId(UserPermit::parse(sampleUserPermit), "4d5a79677065774a7343705272664f72"), sampleHwId);
}

TEST(DataProtection, IssuesTheSameUserPermitAsTheEccLibrary)
{
	EXPECT_EQ(UserPermit::issue(sampleHwId, sampleMKey, sampleMId).toString(), sampleUserPermit);
}

TEST(DataProtection, RejectsAUserPermitWithABadCrc)
{
	std::string damaged(sampleUserPermit);
	damaged[39] = damaged[39] == '1' ? '2' : '1';
	EXPECT_THROW((void)UserPermit::parse(damaged), ProtectionError);
	EXPECT_THROW((void)UserPermit::parse(sampleUserPermit.substr(0, 45)), ProtectionError);
}

// --- data keys and files, against the ECC library ---------------------------------------------

TEST(DataProtection, DecryptsTheDataKeyTheEccLibraryEncrypted)
{
	EXPECT_EQ(toHex(decryptDataKey(javaEncryptedKey, sampleHwId)), javaDataKey);
	EXPECT_EQ(encryptDataKey(fromHex(javaDataKey), sampleHwId), javaEncryptedKey);
}

TEST(DataProtection, DecryptsAFileTheEccLibraryEncrypted)
{
	// The Java archive is deflated and written with a data descriptor (sizes only in the
	// central directory), which the reader has to cope with.
	const auto file = decryptFile(fromHex(javaEncryptedFile), fromHex(javaDataKey));
	EXPECT_EQ(file.entryName, "101KR00TEST.000");
	EXPECT_EQ(textOf(file.content), javaPlainText());
}

TEST(DataProtection, AWrongKeyDoesNotDecrypt)
{
	const auto wrongHwId = "40384B45B54596201114FE9904220102";
	const auto key = decryptDataKey(javaEncryptedKey, wrongHwId);
	EXPECT_THROW((void)decryptFile(fromHex(javaEncryptedFile), key), ProtectionError);
	EXPECT_THROW((void)decryptFile(bytesOf("too short"), fromHex(javaDataKey)), ProtectionError);
}

TEST(DataProtection, EncryptedFilesRoundTrip)
{
	const auto key = fromHex("00112233445566778899AABBCCDDEEFF");
	const auto content = bytesOf(javaPlainText());
	const auto encrypted = encryptFile(content, "101KR00ROUND.000", key);
	EXPECT_NE(encrypted, encryptFile(content, "101KR00ROUND.000", key)) << "the IV is random";

	const auto file = decryptFile(encrypted, key);
	EXPECT_EQ(file.entryName, "101KR00ROUND.000");
	EXPECT_EQ(file.content, content);
}

// --- PERMIT.XML -------------------------------------------------------------------------------

TEST(DataProtection, ReadsPermitXml)
{
	const auto key = fromHex(javaDataKey);
	const auto file = PermitFile::parse(permitXml(sampleUserPermit, sampleHwId,
		{ { "101KR00AAAA.000", key }, { "101KR00BBBB.000", key, "2026-01-31" } }));
	EXPECT_EQ(file.dataServerName, "KHOA");
	EXPECT_EQ(file.dataServerIdentifier, "urn:mrn:iho:KR00:00280");
	EXPECT_EQ(file.issueDate, "2026-11-11");
	EXPECT_EQ(file.userPermit, sampleUserPermit);
	ASSERT_EQ(file.permits.size(), 2u);
	EXPECT_EQ(file.permits[0].product, "S-101");
	EXPECT_EQ(file.permits[0].fileName, "101KR00AAAA.000");
	EXPECT_EQ(file.permits[0].editionNumber, 1);
	EXPECT_EQ(file.permits[0].encryptedKey, javaEncryptedKey);
	EXPECT_EQ(file.permits[1].expiry, "2026-01-31");
}

TEST(DataProtection, ReadsTheEarlierPermitLayout)
{
	const auto file = PermitFile::parse(
		"<Permit><date>20261111 10:00:00</date><dataserver>Test</dataserver>"
		"<userpermit>AD1DAD797C966EC9F6A55B66ED98281599B3C7B1859868</userpermit>"
		"<product id=\"S-102\"><permit><filename>102KR00TEST.h5</filename><editionNumber>2</editionNumber>"
		"<expiry>2027-01-01</expiry><encryptedKey>B31FD6C4D9772D3813709943B4E5C7C9</encryptedKey></permit></product>"
		"</Permit>");
	EXPECT_EQ(file.dataServerName, "Test");
	ASSERT_EQ(file.permits.size(), 1u);
	EXPECT_EQ(file.permits[0].product, "S-102");
	EXPECT_EQ(file.permits[0].editionNumber, 2);
}

TEST(DataProtection, FindsPermitsByNameThenByStem)
{
	const auto key = fromHex(javaDataKey);
	const auto file = PermitFile::parse(permitXml(sampleUserPermit, sampleHwId,
		{ { "101KR00AAAA.000", key }, { "102KR00GRID.H5", key }, { "101KR00DUPE.000", key }, { "101KR00DUPE.001", key } }));
	ASSERT_NE(file.find("101kr00aaaa.000"), nullptr);
	EXPECT_EQ(file.find("101kr00aaaa.000")->fileName, "101KR00AAAA.000");
	ASSERT_NE(file.find("102KR00GRID.h5"), nullptr);
	EXPECT_EQ(file.find("102KR00GRID.hdf5")->fileName, "102KR00GRID.H5");
	EXPECT_EQ(file.find("101KR00DUPE.002"), nullptr) << "two permits share the stem";
	EXPECT_EQ(file.find("CATALOG.XML"), nullptr);
}

TEST(DataProtection, RejectsAFileThatIsNotAPermit)
{
	EXPECT_THROW((void)PermitFile::parse("<Catalog/>"), ProtectionError);
	EXPECT_THROW((void)PermitFile::parse("not xml"), ProtectionError);
}

// --- credentials ------------------------------------------------------------------------------

TEST(DataProtection, ResolvesTheHwIdFromWhatIsGiven)
{
	PermitFile permits;
	permits.userPermit = std::string(sampleUserPermit);

	EXPECT_EQ(resolveCredentials({ .hwId = std::string(sampleHwId) }, permits).hwId, sampleHwId);
	EXPECT_EQ(resolveCredentials({ .manufacturerKey = std::string(sampleMKey) }, permits).hwId, sampleHwId);
	EXPECT_EQ(resolveCredentials({ .manufacturerId = std::string(sampleMId), .manufacturerKey = std::string(sampleMKey),
		.hwId = "40384b45b54596201114fe9904220101", .userPermit = std::string(sampleUserPermit) }, permits).hwId, sampleHwId);

	// A HW_ID alone, with no user permit anywhere, is enough.
	EXPECT_EQ(resolveCredentials({ .hwId = std::string(sampleHwId) }, PermitFile{}).hwId, sampleHwId);
}

TEST(DataProtection, RejectsCredentialsThatDisagree)
{
	PermitFile permits;
	permits.userPermit = std::string(sampleUserPermit);
	const std::string otherHwId = "40384B45B54596201114FE9904220102";

	EXPECT_THROW((void)resolveCredentials({ .manufacturerKey = std::string(sampleMKey), .hwId = otherHwId }, permits), ProtectionError);
	EXPECT_THROW((void)resolveCredentials({ .manufacturerId = "123456", .hwId = std::string(sampleHwId) }, permits), ProtectionError);
	EXPECT_THROW((void)resolveCredentials({ .hwId = std::string(sampleHwId),
		.userPermit = UserPermit::issue(otherHwId, sampleMKey, sampleMId).toString() }, permits), ProtectionError);
	EXPECT_THROW((void)resolveCredentials({}, permits), ProtectionError);
	EXPECT_THROW((void)resolveCredentials({ .manufacturerKey = std::string(sampleMKey) }, PermitFile{}), ProtectionError);
	EXPECT_THROW((void)resolveCredentials({ .hwId = "not hex" }, permits), ProtectionError);
}

// --- installing -------------------------------------------------------------------------------

namespace
{
	// Reads a "dataset" when its content starts with "CELL", so a file that did not decrypt is
	// rejected as a real reader would reject it.
	class TextCellInspector final : public DatasetInspector
	{
	public:
		bool accepts(const fs::path& file) const override { return file.extension() == ".000"; }

		std::optional<InspectedDataset> inspect(const fs::path& file) const override
		{
			const auto content = textOf(readBytes(file));
			if (!content.starts_with("CELL"))
			{
				return std::nullopt;
			}
			return InspectedDataset{ .name = file.stem().string(), .product = "S-101", .productEdition = "2.0.0",
				.edition = 1, .issueDate = "2026-11-11",
				.coverages = { InspectedCoverage{ .optimumDisplayScale = 22000, .minimumDisplayScale = 90000,
					.rings = { { { 126, 34 }, { 127, 34 }, { 127, 35 }, { 126, 35 } } } } } };
		}
	};

	// CATALOG.XML here is one dataset file name per line, under S-101/DATASET_FILES.
	class LineCatalogueReader final : public ExchangeSetReader
	{
	public:
		std::optional<InspectedExchangeSet> read(const fs::path& catalogXml) const override
		{
			InspectedExchangeSet set{ .identifier = "TEST", .issueDate = "2026-11-11" };
			std::istringstream lines(textOf(readBytes(catalogXml)));
			for (std::string line; std::getline(lines, line);)
			{
				if (!line.empty())
				{
					set.datasets.push_back({ .file = catalogXml.parent_path() / "S-101" / "DATASET_FILES" / line, .product = "S-101" });
				}
			}
			return set;
		}
	};

	class ProtectedImportTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
			dir = testpaths::tempDir() / L"ProtectedImport" / info->name();
			fs::remove_all(dir);
			fs::create_directories(dir);
			store = std::make_unique<ResourceStore>(dir / "root");
			importer = std::make_unique<Importer>(database, *store, Importer::Readers{ { &cells }, nullptr, &catalogue },
				[] { return std::string("2026-11-11T00:00:00Z"); });
		}

		// An exchange set of two protected cells (and an unprotected README) under dir/source.
		fs::path makeExchangeSet(std::string_view hwIdInPermit = sampleHwId)
		{
			const fs::path source = dir / "source";
			writeText(source / "CATALOG.XML", "101KR00AAAA.000\n101KR00BBBB.000\n");
			writeText(source / "README.TXT", "not protected");
			for (const auto& name : { "101KR00AAAA.000", "101KR00BBBB.000" })
			{
				writeBytes(source / "S-101" / "DATASET_FILES" / name, encryptFile(plain(name), name, key));
			}
			writeText(dir / "PERMIT.XML", permitXml(sampleUserPermit, hwIdInPermit,
				{ { "101KR00AAAA.000", key }, { "101KR00BBBB.000", key, "2026-01-31" } }));
			return source;
		}

		static Bytes plain(std::string_view name) { return bytesOf(std::format("CELL {} contents", name)); }

		ProtectedImportRequest request(fs::path source) const
		{
			return { .manufacturerId = std::string(sampleMId), .manufacturerKey = std::string(sampleMKey),
				.hwId = std::string(sampleHwId), .userPermit = std::string(sampleUserPermit),
				.permitFile = dir / "PERMIT.XML", .source = std::move(source) };
		}

		ProtectedImportOptions options() const { return { .stagingParent = dir / "staging", .today = "2026-11-11" }; }

		// The installed copy of a dataset, as the store holds it.
		Bytes installed(std::string_view name) const
		{
			const auto record = database.datasets().find("S-101", name);
			return record ? readBytes(store->absolute(record->path)) : Bytes{};
		}

		fs::path dir;
		SystemDatabase database = SystemDatabase::openInMemory();
		std::unique_ptr<ResourceStore> store;
		TextCellInspector cells;
		LineCatalogueReader catalogue;
		std::unique_ptr<Importer> importer;
		Bytes key = fromHex("00112233445566778899AABBCCDDEEFF");
	};
}

TEST_F(ProtectedImportTest, InstallsAProtectedExchangeSetFolder)
{
	const auto source = makeExchangeSet();
	const auto report = importProtected(*importer, request(source), options());

	EXPECT_EQ(report.hwId, sampleHwId);
	EXPECT_EQ(report.dataServerName, "KHOA");
	EXPECT_EQ(report.decryptedCount(), 2u);
	EXPECT_EQ(report.import.count(ImportOutcome::Added), 2u);
	EXPECT_EQ(installed("101KR00AAAA"), plain("101KR00AAAA.000"));
	EXPECT_EQ(installed("101KR00BBBB"), plain("101KR00BBBB.000"));

	// The expired permit is reported; its data is installed all the same.
	ASSERT_EQ(report.warnings.size(), 1u);
	EXPECT_NE(report.warnings[0].find("101KR00BBBB.000 expired on 2026-01-31"), std::string::npos);

	// Results name the source, and the staging folder is gone.
	for (const auto& result : report.import.results)
	{
		EXPECT_EQ(result.source.parent_path(), source / "S-101" / "DATASET_FILES") << result.source;
	}
	EXPECT_TRUE(fs::is_empty(dir / "staging"));
}

TEST_F(ProtectedImportTest, InstallsFromAZipWithThePermitInside)
{
	makeExchangeSet();
	const fs::path source = dir / "source";
	std::vector<std::pair<std::string, Bytes>> entries;
	for (const auto& name : { "CATALOG.XML", "S-101/DATASET_FILES/101KR00AAAA.000", "S-101/DATASET_FILES/101KR00BBBB.000" })
	{
		entries.emplace_back(std::string("EXSET/") + name, readBytes(source / name));
	}
	entries.emplace_back("EXSET/INFO/PERMIT.XML", readBytes(dir / "PERMIT.XML"));
	entries.emplace_back("../escape.000", plain("escape.000"));
	writeBytes(dir / "exset.zip", storedZip(entries));

	auto req = request(dir / "exset.zip");
	req.permitFile.clear();      // found in the archive
	req.hwId.clear();            // decrypted from the user permit with M_KEY
	const auto report = importProtected(*importer, req, options());

	EXPECT_EQ(report.decryptedCount(), 2u);
	EXPECT_EQ(report.import.count(ImportOutcome::Added), 2u);
	EXPECT_EQ(installed("101KR00AAAA"), plain("101KR00AAAA.000"));
	EXPECT_TRUE(std::ranges::any_of(report.warnings, [](const std::string& w) { return w.find("../escape.000") != std::string::npos; }));
}

TEST_F(ProtectedImportTest, InstallsASingleProtectedFile)
{
	makeExchangeSet();
	const auto report = importProtected(*importer, request(dir / "source" / "S-101" / "DATASET_FILES" / "101KR00AAAA.000"), options());

	ASSERT_EQ(report.files.size(), 1u);
	EXPECT_TRUE(report.files[0].decrypted);
	ASSERT_EQ(report.import.results.size(), 1u);
	EXPECT_EQ(report.import.results[0].outcome, ImportOutcome::Added);
	EXPECT_EQ(installed("101KR00AAAA"), plain("101KR00AAAA.000"));
	// The other permit has no file here.
	EXPECT_TRUE(std::ranges::any_of(report.warnings, [](const std::string& w) { return w.find("101KR00BBBB.000, which") != std::string::npos; }));
}

TEST_F(ProtectedImportTest, AFileForAnotherSystemIsReportedAndNotInstalled)
{
	// The permit was issued for another HW_ID: the data keys do not decrypt with ours.
	const auto source = makeExchangeSet("40384B45B54596201114FE9904220102");
	auto req = request(source);
	req.manufacturerKey.clear();
	const auto report = importProtected(*importer, req, options());

	EXPECT_EQ(report.decryptedCount(), 0u);
	ASSERT_EQ(report.files.size(), 2u);
	EXPECT_NE(report.files[0].message.find("does not decrypt"), std::string::npos);
	EXPECT_EQ(report.import.count(ImportOutcome::Rejected), 2u);
	EXPECT_TRUE(database.datasets().all().empty());
}

TEST_F(ProtectedImportTest, RejectsARequestThatCannotWork)
{
	const auto source = makeExchangeSet();

	auto noPermit = request(source);
	noPermit.permitFile.clear();
	EXPECT_THROW((void)importProtected(*importer, noPermit, options()), ProtectionError) << "no PERMIT.XML in the folder";

	auto wrongHwId = request(source);
	wrongHwId.hwId = "40384B45B54596201114FE9904220102";
	EXPECT_THROW((void)importProtected(*importer, wrongHwId, options()), ProtectionError) << "HW_ID disagrees with M_KEY";

	EXPECT_THROW((void)importProtected(*importer, request(dir / "missing"), options()), ProtectionError);
	EXPECT_TRUE(database.datasets().all().empty());
}

TEST_F(ProtectedImportTest, ReadsExpiryDatesWithoutDashes)
{
	// The earlier permit layout (KHOA test data of 2020) writes the expiry as yyyyMMdd.
	const auto source = makeExchangeSet();
	std::string xml = textOf(readBytes(dir / "PERMIT.XML"));
	xml.replace(xml.find("2026-01-31"), 10, "20260131");
	writeText(dir / "PERMIT.XML", xml);

	const auto report = importProtected(*importer, request(source), options());
	ASSERT_EQ(report.warnings.size(), 1u);
	EXPECT_NE(report.warnings[0].find("expired on 20260131"), std::string::npos);
}

// --- real protected data, with the readers the viewer uses ------------------------------------

namespace
{
	std::wstring environment(const wchar_t* name)
	{
#ifdef _WIN32
		// Wide: the paths of the Korean test data do not survive the ANSI code page.
		wchar_t* value = nullptr;
		std::size_t length = 0;
		if (_wdupenv_s(&value, &length, name) != 0 || !value)
		{
			return {};
		}
		std::wstring text(value);
		std::free(value);
		return text;
#else
		(void)name;
		return {};
#endif
	}

	std::string environmentUtf8(const wchar_t* name)
	{
		const auto value = fs::path(environment(name)).u8string();
		return std::string(value.begin(), value.end());
	}
}

// Installs protected data named by the environment into a scratch System Database and prints
// the report. Skipped unless OPENS100_P15_SOURCE is set.
//
//   OPENS100_P15_SOURCE      exchange set folder, its CATALOG.XML, a .zip, or one protected file
//   OPENS100_P15_PERMIT      PERMIT.XML (optional: else the one in the source)
//   OPENS100_P15_MID, _MKEY, _HWID, _USERPERMIT    the credentials (as many as you have)
//   OPENS100_P15_STORE       keep the store here instead of a scratch folder (optional)
TEST(ProtectedImportReal, InstallsTheDataNamedByTheEnvironment)
{
	const fs::path source = environment(L"OPENS100_P15_SOURCE");
	if (source.empty())
	{
		GTEST_SKIP() << "OPENS100_P15_SOURCE is not set";
	}
	fs::path root = environment(L"OPENS100_P15_STORE");
	if (root.empty())
	{
		root = testpaths::tempDir() / L"ProtectedImportReal";
		fs::remove_all(root);
	}

	ResourceStore store(root);
	SystemDatabase database = SystemDatabase::open(store.databaseFile());
	const GISLibrary::S101DatasetInspector cells;
	const GISLibrary::Hdf5DatasetInspector grids;
	const GISLibrary::XmlCatalogueInspector catalogues;
	const GISLibrary::ExchangeCatalogueReader exchangeSets;
	Importer importer(database, store, Importer::Readers{ { &cells, &grids }, &catalogues, &exchangeSets });

	const ProtectedImportRequest request{
		.manufacturerId = environmentUtf8(L"OPENS100_P15_MID"),
		.manufacturerKey = environmentUtf8(L"OPENS100_P15_MKEY"),
		.hwId = environmentUtf8(L"OPENS100_P15_HWID"),
		.userPermit = environmentUtf8(L"OPENS100_P15_USERPERMIT"),
		.permitFile = environment(L"OPENS100_P15_PERMIT"),
		.source = source,
	};
	const auto report = importProtected(importer, request, { .stagingParent = root.parent_path() });

	std::cout << "HW_ID " << report.hwId << ", data server " << report.dataServerName << "\n"
		<< "decrypted " << report.decryptedCount() << " of " << report.files.size() << " protected files\n";
	for (const auto& file : report.files)
	{
		if (!file.decrypted)
		{
			std::cout << "  NOT DECRYPTED " << file.file << ": " << file.message << "\n";
		}
	}
	for (const auto outcome : { ImportOutcome::Added, ImportOutcome::Replaced, ImportOutcome::UpdateAdded,
		ImportOutcome::Cancelled, ImportOutcome::Unchanged, ImportOutcome::Rejected })
	{
		std::cout << "  " << toString(outcome) << ": " << report.import.count(outcome) << "\n";
	}
	for (const auto& result : report.import.results)
	{
		if (result.outcome == ImportOutcome::Rejected)
		{
			std::cout << "  rejected " << toUtf8(result.source.filename()) << ": " << result.message << "\n";
		}
	}
	for (const auto& warning : report.warnings)
	{
		std::cout << "  warning: " << warning << "\n";
	}
	std::cout << "store: " << toUtf8(root) << "\n";

	EXPECT_GT(report.decryptedCount(), 0u);
	EXPECT_EQ(report.decryptedCount(), report.files.size());
}

// The earlier layout (KHOA's 2020 test data sets): no IV in the file, the whole file encrypted
// with a zero IV. Made here directly with OpenSSL.
TEST(DataProtection, DecryptsTheZeroIvLayout)
{
	const auto key = fromHex("9B87060DAF0B0B0B0B0B0B0B0B0B0B0B");
	const auto content = bytesOf(javaPlainText());
	const auto zip = storedZip({ { "101KR00LEGACY.000", content } });

	EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
	const std::array<std::uint8_t, 16> zeroIv{};
	Bytes encrypted(zip.size() + 16);
	int written = 0;
	int last = 0;
	ASSERT_EQ(EVP_EncryptInit_ex(context, EVP_aes_128_cbc(), nullptr, key.data(), zeroIv.data()), 1);
	ASSERT_EQ(EVP_EncryptUpdate(context, encrypted.data(), &written, zip.data(), static_cast<int>(zip.size())), 1);
	ASSERT_EQ(EVP_EncryptFinal_ex(context, encrypted.data() + written, &last), 1);
	EVP_CIPHER_CTX_free(context);
	encrypted.resize(static_cast<std::size_t>(written + last));

	const auto file = decryptFile(encrypted, key);
	EXPECT_EQ(file.entryName, "101KR00LEGACY.000");
	EXPECT_EQ(file.content, content);
}

// A container that ends after its first entry, with no central directory (as in KHOA's 2020
// test data, which is also encrypted with a zero IV).
TEST(DataProtection, ReadsAContainerWithoutACentralDirectory)
{
	const auto content = bytesOf(javaPlainText());
	auto zip = storedZip({ { "5bb9a4b8-5065-423e-94f4-3176be8bf584", content } });
	zip.resize(30 + 36 + content.size());   // local header, name, data
	EXPECT_THROW(ZipArchive{ zip }, ZipError);

	const auto file = ZipArchive::readFirstLocalFile(zip);
	EXPECT_EQ(file.name, "5bb9a4b8-5065-423e-94f4-3176be8bf584");
	EXPECT_EQ(file.content, content);
}
