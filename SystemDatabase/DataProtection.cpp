#include "DataProtection.h"

#include "ZipArchive.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <pugixml.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <format>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>

namespace s100::sdb
{
    namespace
    {
        constexpr std::size_t blockSize = 16;   // AES block, key and IV length

        std::string_view trim(std::string_view text)
        {
            const auto first = text.find_first_not_of(" \t\r\n");
            if (first == std::string_view::npos)
            {
                return {};
            }
            const auto last = text.find_last_not_of(" \t\r\n");
            return text.substr(first, last - first + 1);
        }

        bool equalsIgnoreCase(std::string_view a, std::string_view b)
        {
            return std::ranges::equal(a, b, [](char x, char y) {
                return std::toupper(static_cast<unsigned char>(x)) == std::toupper(static_cast<unsigned char>(y));
            });
        }

        std::string_view stem(std::string_view fileName)
        {
            const auto dot = fileName.rfind('.');
            return dot == std::string_view::npos || dot == 0 ? fileName : fileName.substr(0, dot);
        }

        struct CipherContextDeleter
        {
            void operator()(EVP_CIPHER_CTX* context) const noexcept { EVP_CIPHER_CTX_free(context); }
        };

        // AES-128-CBC over `input`. Returns nothing when OpenSSL reports a failure, which for a
        // decryption with padding is how a wrong key shows itself.
        std::optional<Bytes> aes128Cbc(bool encrypt, std::span<const std::uint8_t> key, std::span<const std::uint8_t> iv,
            bool padding, std::span<const std::uint8_t> input)
        {
            if (key.size() != blockSize || iv.size() != blockSize
                || input.size() > static_cast<std::size_t>(std::numeric_limits<int>::max() - static_cast<int>(blockSize)))
            {
                return std::nullopt;
            }
            const std::unique_ptr<EVP_CIPHER_CTX, CipherContextDeleter> context(EVP_CIPHER_CTX_new());
            if (!context
                || EVP_CipherInit_ex(context.get(), EVP_aes_128_cbc(), nullptr, key.data(), iv.data(), encrypt ? 1 : 0) != 1
                || EVP_CIPHER_CTX_set_padding(context.get(), padding ? 1 : 0) != 1)
            {
                return std::nullopt;
            }

            Bytes output(input.size() + blockSize);
            int written = 0;
            if (EVP_CipherUpdate(context.get(), output.data(), &written, input.data(), static_cast<int>(input.size())) != 1)
            {
                return std::nullopt;
            }
            int last = 0;
            if (EVP_CipherFinal_ex(context.get(), output.data() + written, &last) != 1)
            {
                return std::nullopt;
            }
            output.resize(static_cast<std::size_t>(written) + static_cast<std::size_t>(last));
            return output;
        }

        // One block with a zero IV and no padding: how Part 15 encrypts a HW_ID and a data key.
        Bytes blockCipher(bool encrypt, std::span<const std::uint8_t> key, std::span<const std::uint8_t> block)
        {
            const std::array<std::uint8_t, blockSize> zeroIv{};
            auto result = aes128Cbc(encrypt, key, zeroIv, false, block);
            if (!result || result->size() != blockSize)
            {
                throw ProtectionError("AES-128 block operation failed");
            }
            return *std::move(result);
        }

        void put16(Bytes& out, std::uint32_t value)
        {
            out.push_back(static_cast<std::uint8_t>(value));
            out.push_back(static_cast<std::uint8_t>(value >> 8));
        }

        void put32(Bytes& out, std::uint32_t value)
        {
            put16(out, value & 0xFFFF);
            put16(out, value >> 16);
        }

        // A ZIP archive with one stored entry: the container encryptFile wraps a file in.
        Bytes storedZip(std::span<const std::uint8_t> content, std::string_view name)
        {
            if (content.size() >= 0xFFFFFFFF || name.size() > 0xFFFF)
            {
                throw ProtectionError("file too large for a ZIP container without ZIP64");
            }
            const auto size = static_cast<std::uint32_t>(content.size());
            const auto nameLength = static_cast<std::uint32_t>(name.size());
            const std::uint32_t crc = computeCrc32(content);
            constexpr std::uint32_t utf8Names = 0x0800;

            Bytes zip;
            zip.reserve(content.size() + 2 * name.size() + 100);
            put32(zip, 0x04034b50);
            put16(zip, 20);              // version needed
            put16(zip, utf8Names);
            put16(zip, 0);               // stored
            put32(zip, 0);               // time, date
            put32(zip, crc);
            put32(zip, size);
            put32(zip, size);
            put16(zip, nameLength);
            put16(zip, 0);
            zip.insert(zip.end(), name.begin(), name.end());
            zip.insert(zip.end(), content.begin(), content.end());

            const auto directoryOffset = static_cast<std::uint32_t>(zip.size());
            put32(zip, 0x02014b50);
            put16(zip, 20);              // version made by
            put16(zip, 20);
            put16(zip, utf8Names);
            put16(zip, 0);
            put32(zip, 0);
            put32(zip, crc);
            put32(zip, size);
            put32(zip, size);
            put16(zip, nameLength);
            put16(zip, 0);               // extra
            put16(zip, 0);               // comment
            put16(zip, 0);               // disk
            put16(zip, 0);               // internal attributes
            put32(zip, 0);               // external attributes
            put32(zip, 0);               // local header offset
            zip.insert(zip.end(), name.begin(), name.end());
            const auto directorySize = static_cast<std::uint32_t>(zip.size()) - directoryOffset;

            put32(zip, 0x06054b50);
            put16(zip, 0);
            put16(zip, 0);
            put16(zip, 1);
            put16(zip, 1);
            put32(zip, directorySize);
            put32(zip, directoryOffset);
            put16(zip, 0);
            return zip;
        }

        std::string localName(const pugi::xml_node& node)
        {
            const std::string_view name = node.name();
            const auto colon = name.find(':');
            return std::string(colon == std::string_view::npos ? name : name.substr(colon + 1));
        }

        std::string text(const pugi::xml_node& node)
        {
            return std::string(trim(node.text().get()));
        }

        DatasetPermit readDatasetPermit(const pugi::xml_node& node, const std::string& product)
        {
            DatasetPermit permit{ .product = product };
            for (const auto& child : node.children())
            {
                const auto name = localName(child);
                if (name == "filename")
                {
                    permit.fileName = text(child);
                }
                else if (name == "editionNumber")
                {
                    const auto value = text(child);
                    std::from_chars(value.data(), value.data() + value.size(), permit.editionNumber);
                }
                else if (name == "issueDate")
                {
                    permit.issueDate = text(child);
                }
                else if (name == "expiry")
                {
                    permit.expiry = text(child);
                }
                else if (name == "encryptedKey")
                {
                    permit.encryptedKey = text(child);
                }
            }
            return permit;
        }
    }

    Bytes fromHex(std::string_view hex)
    {
        hex = trim(hex);
        if (hex.size() % 2 != 0)
        {
            throw ProtectionError("hex text has an odd number of digits");
        }
        Bytes bytes(hex.size() / 2);
        for (std::size_t i = 0; i < bytes.size(); ++i)
        {
            const char* first = hex.data() + 2 * i;
            const auto [end, error] = std::from_chars(first, first + 2, bytes[i], 16);
            if (error != std::errc{} || end != first + 2)
            {
                throw ProtectionError(std::format("'{}' is not hex", std::string_view(first, 2)));
            }
        }
        return bytes;
    }

    std::string toHex(std::span<const std::uint8_t> bytes)
    {
        static constexpr char digits[] = "0123456789ABCDEF";
        std::string hex;
        hex.reserve(bytes.size() * 2);
        for (const auto byte : bytes)
        {
            hex.push_back(digits[byte >> 4]);
            hex.push_back(digits[byte & 0x0F]);
        }
        return hex;
    }

    std::string normaliseKey(std::string_view hex, std::string_view what)
    {
        hex = trim(hex);
        if (hex.size() != 2 * blockSize || !std::ranges::all_of(hex, [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }))
        {
            throw ProtectionError(std::format("{} must be 32 hex digits", what));
        }
        std::string key(hex);
        std::ranges::transform(key, key.begin(), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
        return key;
    }

    UserPermit UserPermit::parse(std::string_view text)
    {
        text = trim(text);
        if (text.size() != 46)
        {
            throw ProtectionError(std::format("a user permit has 46 characters, not {}", text.size()));
        }
        UserPermit permit{
            .encryptedHwId = normaliseKey(text.substr(0, 32), "the encrypted HW_ID of a user permit"),
            .checksum = std::string(text.substr(32, 8)),
            .manufacturerId = std::string(text.substr(40, 6)),
        };

        // The CRC is computed over the encrypted HW_ID characters as written.
        const auto hwIdText = text.substr(0, 32);
        const std::uint32_t expected = computeCrc32({ reinterpret_cast<const std::uint8_t*>(hwIdText.data()), hwIdText.size() });
        std::uint32_t written = 0;
        const auto [end, error] = std::from_chars(permit.checksum.data(), permit.checksum.data() + permit.checksum.size(), written, 16);
        if (error != std::errc{} || end != permit.checksum.data() + permit.checksum.size() || written != expected)
        {
            throw ProtectionError("the user permit fails its CRC check");
        }
        std::ranges::transform(permit.checksum, permit.checksum.begin(),
            [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
        return permit;
    }

    UserPermit UserPermit::issue(std::string_view hwId, std::string_view manufacturerKey, std::string_view manufacturerId)
    {
        if (manufacturerId.size() != 6)
        {
            throw ProtectionError("an M_ID has 6 characters");
        }
        const auto key = fromHex(normaliseKey(manufacturerKey, "M_KEY"));
        const auto plain = fromHex(normaliseKey(hwId, "HW_ID"));
        const std::string encrypted = toHex(blockCipher(true, key, plain));
        const std::uint32_t crc = computeCrc32({ reinterpret_cast<const std::uint8_t*>(encrypted.data()), encrypted.size() });
        return UserPermit{ .encryptedHwId = encrypted, .checksum = std::format("{:08X}", crc), .manufacturerId = std::string(manufacturerId) };
    }

    std::string UserPermit::toString() const
    {
        return encryptedHwId + checksum + manufacturerId;
    }

    std::string decryptHwId(const UserPermit& permit, std::string_view manufacturerKey)
    {
        const auto key = fromHex(normaliseKey(manufacturerKey, "M_KEY"));
        return toHex(blockCipher(false, key, fromHex(permit.encryptedHwId)));
    }

    PermitFile PermitFile::parse(std::string_view xml)
    {
        pugi::xml_document document;
        const auto result = document.load_buffer(xml.data(), xml.size());
        if (!result)
        {
            throw ProtectionError(std::format("PERMIT.XML is not well-formed XML: {}", result.description()));
        }
        const auto root = document.document_element();
        if (!equalsIgnoreCase(localName(root), "Permit"))
        {
            throw ProtectionError(std::format("the root element of PERMIT.XML is <{}>, not <Permit>", root.name()));
        }

        PermitFile file;
        // Header fields and products are found wherever they are nested, which covers both
        // layouts; permits keep document order.
        const auto walk = [&file](const auto& self, const pugi::xml_node& node, const std::string& product) -> void {
            for (const auto& child : node.children())
            {
                if (child.type() != pugi::node_element)
                {
                    continue;
                }
                const auto name = localName(child);
                if (name == "datasetPermit" || name == "permit")
                {
                    file.permits.push_back(readDatasetPermit(child, product));
                }
                else if (name == "product")
                {
                    self(self, child, child.attribute("id").as_string());
                }
                else if (name == "issueDate" || name == "date")
                {
                    file.issueDate = text(child);
                }
                else if (name == "dataServerName" || name == "dataserver")
                {
                    file.dataServerName = text(child);
                }
                else if (name == "dataServerIdentifier")
                {
                    file.dataServerIdentifier = text(child);
                }
                else if (name == "version")
                {
                    file.version = text(child);
                }
                else if (name == "userpermit")
                {
                    file.userPermit = text(child);
                }
                else
                {
                    self(self, child, product);
                }
            }
        };
        walk(walk, root, std::string());
        return file;
    }

    PermitFile PermitFile::load(const std::filesystem::path& file)
    {
        std::ifstream in(file, std::ios::binary);
        if (!in)
        {
            throw ProtectionError("cannot open PERMIT.XML");
        }
        const std::string xml{ std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
        if (in.bad())
        {
            throw ProtectionError("cannot read PERMIT.XML");
        }
        return parse(xml);
    }

    const DatasetPermit* PermitFile::find(std::string_view fileName) const
    {
        for (const auto& permit : permits)
        {
            if (equalsIgnoreCase(permit.fileName, fileName))
            {
                return &permit;
            }
        }
        const DatasetPermit* byStem = nullptr;
        for (const auto& permit : permits)
        {
            if (equalsIgnoreCase(stem(permit.fileName), stem(fileName)))
            {
                if (byStem)
                {
                    return nullptr;   // ambiguous
                }
                byStem = &permit;
            }
        }
        return byStem;
    }

    Bytes decryptDataKey(std::string_view encryptedKey, std::string_view hwId)
    {
        const auto key = fromHex(normaliseKey(hwId, "HW_ID"));
        return blockCipher(false, key, fromHex(normaliseKey(encryptedKey, "the encrypted data key")));
    }

    std::string encryptDataKey(std::span<const std::uint8_t> dataKey, std::string_view hwId)
    {
        if (dataKey.size() != blockSize)
        {
            throw ProtectionError("a data key has 16 bytes");
        }
        return toHex(blockCipher(true, fromHex(normaliseKey(hwId, "HW_ID")), dataKey));
    }

    DecryptedFile decryptFile(std::span<const std::uint8_t> encrypted, std::span<const std::uint8_t> dataKey)
    {
        if (dataKey.size() != blockSize)
        {
            throw ProtectionError("a data key has 16 bytes");
        }
        if (encrypted.size() < blockSize || encrypted.size() % blockSize != 0)
        {
            throw ProtectionError("the file is not AES-128-CBC encrypted data (wrong length)");
        }

        // Two layouts are in use. Current data servers (and the ECC library) put a random IV in
        // the first 16 bytes. Earlier data (KHOA's 2020 test data sets) has no IV in the file:
        // the whole file is encrypted with a zero IV. Only the right one yields a ZIP header, so
        // they are tried in turn.
        std::optional<Bytes> zip;
        if (encrypted.size() >= 2 * blockSize)
        {
            zip = aes128Cbc(false, dataKey, encrypted.first(blockSize), true, encrypted.subspan(blockSize));
        }
        if (!zip || !ZipArchive::looksLikeZip(*zip))
        {
            const std::array<std::uint8_t, blockSize> zeroIv{};
            zip = aes128Cbc(false, dataKey, zeroIv, true, encrypted);
            if (!zip)
            {
                zip = aes128Cbc(false, dataKey, zeroIv, false, encrypted);   // unpadded: the ZIP reader ignores the tail
            }
        }
        if (!zip || !ZipArchive::looksLikeZip(*zip))
        {
            throw ProtectionError("the file does not decrypt with its permit (wrong HW_ID or data key, or a damaged file)");
        }

        std::optional<ZipArchive> parsed;
        try
        {
            parsed.emplace(*zip);
        }
        catch (const ZipError&)
        {
            // No central directory (the 2020 KHOA test data): read the entry from its local header.
            try
            {
                auto local = ZipArchive::readFirstLocalFile(*zip);
                return DecryptedFile{ .entryName = std::move(local.name), .content = std::move(local.content) };
            }
            catch (const ZipError& error)
            {
                throw ProtectionError(std::format("the decrypted container is damaged: {}", error.what()));
            }
        }

        try
        {
            const ZipArchive& archive = *parsed;
            const auto entry = std::ranges::find_if(archive.entries(), [](const ZipArchive::Entry& e) { return !e.isDirectory(); });
            if (entry == archive.entries().end())
            {
                throw ProtectionError("the decrypted container holds no file");
            }
            return DecryptedFile{ .entryName = entry->name, .content = archive.read(*entry) };
        }
        catch (const ZipError& error)
        {
            throw ProtectionError(std::format("the decrypted container is damaged: {}", error.what()));
        }
    }

    Bytes encryptFile(std::span<const std::uint8_t> content, std::string_view entryName, std::span<const std::uint8_t> dataKey)
    {
        std::array<std::uint8_t, blockSize> iv{};
        if (RAND_bytes(iv.data(), static_cast<int>(iv.size())) != 1)
        {
            throw ProtectionError("no random IV available");
        }
        const auto zip = storedZip(content, entryName);
        auto encrypted = aes128Cbc(true, dataKey, iv, true, zip);
        if (!encrypted)
        {
            throw ProtectionError("AES-128-CBC encryption failed");
        }
        Bytes result(iv.begin(), iv.end());
        result.insert(result.end(), encrypted->begin(), encrypted->end());
        return result;
    }
}
