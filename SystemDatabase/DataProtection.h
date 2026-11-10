#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// S-100 Part 15 data protection: the user permit, PERMIT.XML and the decryption of protected
// files, ported from the secom-client tool (which uses the ECC no.ecc.s100.security library) and
// checked against vectors produced by that library (Tests/OpenS100Tests/DataProtectionTest.cpp).
//
//   M_KEY  (manufacturer key, 32 hex)  --AES-128-CBC, zero IV, no padding-->  encrypted HW_ID
//   USERPERMIT = encrypted HW_ID (32 hex) + CRC-32 of those 32 characters (8 hex) + M_ID (6)
//   HW_ID  (32 hex)  --AES-128-CBC, zero IV, no padding-->  encryptedKey in PERMIT.XML
//   data key  --AES-128-CBC, PKCS#7, IV in the first 16 bytes-->  protected file
//   protected file, decrypted  =  a ZIP archive holding the original file as its entry
//
// Digital signatures (the other half of Part 15) are verified elsewhere (SDB-8).
namespace s100::sdb
{
    class ProtectionError final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    using Bytes = std::vector<std::uint8_t>;

    // Hex text to bytes and back. Hex input may be either case and is trimmed of surrounding
    // white space; anything else throws ProtectionError.
    [[nodiscard]] Bytes fromHex(std::string_view hex);
    [[nodiscard]] std::string toHex(std::span<const std::uint8_t> bytes);   // upper case
    // An AES-128 key or a HW_ID: exactly 32 hex digits, returned upper case. Throws
    // ProtectionError naming `what` otherwise.
    [[nodiscard]] std::string normaliseKey(std::string_view hex, std::string_view what);

    struct UserPermit
    {
        std::string encryptedHwId;   // 32 hex digits
        std::string checksum;        // 8 hex digits: CRC-32 of the encryptedHwId characters
        std::string manufacturerId;  // M_ID, 6 characters

        // Splits a 46-character user permit and checks its CRC. Throws ProtectionError.
        [[nodiscard]] static UserPermit parse(std::string_view text);
        // Encrypts a HW_ID with a manufacturer key into a user permit (what a manufacturer issues;
        // used by the tests and by tools that make test permits).
        [[nodiscard]] static UserPermit issue(std::string_view hwId, std::string_view manufacturerKey, std::string_view manufacturerId);
        [[nodiscard]] std::string toString() const;
    };

    // The HW_ID the user permit was issued for, recovered with the manufacturer key (M_KEY).
    // Throws ProtectionError for a malformed key.
    [[nodiscard]] std::string decryptHwId(const UserPermit& permit, std::string_view manufacturerKey);

    struct DatasetPermit
    {
        std::string product;         // the <product id="..."> it is listed under ("S-101")
        std::string fileName;
        int editionNumber = 0;
        std::string issueDate;       // as written, may be empty
        std::string expiry;          // yyyy-MM-dd as written, may be empty
        std::string encryptedKey;    // the data key encrypted with the HW_ID, 32 hex
    };

    // PERMIT.XML (S-100 Part 15, the permit file of an exchange set). Reads the layout data servers
    // write today (<header><issueDate/><dataServerName/>...<userpermit/></header>,
    // <products><product id="..."><datasetPermit>...) and the earlier one the ECC library also
    // accepts (<date>, <dataserver>, <permit>). Element names are matched without their prefix.
    struct PermitFile
    {
        std::string issueDate;
        std::string dataServerName;
        std::string dataServerIdentifier;
        std::string version;
        std::string userPermit;
        std::vector<DatasetPermit> permits;

        // Throws ProtectionError when the file cannot be read or is not a permit file.
        [[nodiscard]] static PermitFile load(const std::filesystem::path& file);
        [[nodiscard]] static PermitFile parse(std::string_view xml);

        // The permit for a file name: an exact match (ignoring case) first, then the only permit
        // whose name without its extension matches (secom-client does the same, for exchange sets
        // whose files were renamed by extension). Null when there is none.
        [[nodiscard]] const DatasetPermit* find(std::string_view fileName) const;
    };

    // The data key of a dataset permit, decrypted with the HW_ID. Throws ProtectionError.
    [[nodiscard]] Bytes decryptDataKey(std::string_view encryptedKey, std::string_view hwId);

    struct DecryptedFile
    {
        std::string entryName;       // the name the file had inside its ZIP container
        Bytes content;
    };

    // A protected file decrypted with its data key and taken out of its ZIP container. Throws
    // ProtectionError when the key is wrong (bad padding, no ZIP inside) or the file is damaged.
    [[nodiscard]] DecryptedFile decryptFile(std::span<const std::uint8_t> encrypted, std::span<const std::uint8_t> dataKey);

    // The inverse of decryptFile, with a random IV: what a data server does. For the tests and for
    // tools that make protected test data.
    [[nodiscard]] Bytes encryptFile(std::span<const std::uint8_t> content, std::string_view entryName, std::span<const std::uint8_t> dataKey);
    // The encryptedKey a data server writes into PERMIT.XML for a data key and a HW_ID.
    [[nodiscard]] std::string encryptDataKey(std::span<const std::uint8_t> dataKey, std::string_view hwId);
}
