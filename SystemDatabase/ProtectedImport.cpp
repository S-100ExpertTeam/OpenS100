#include "ProtectedImport.h"

#include "Utf8Path.h"
#include "ZipArchive.h"

#include <openssl/rand.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <format>
#include <fstream>
#include <iterator>
#include <set>

namespace s100::sdb
{
    namespace fs = std::filesystem;

    namespace
    {
        std::string trimmed(std::string_view text)
        {
            const auto first = text.find_first_not_of(" \t\r\n");
            if (first == std::string_view::npos)
            {
                return {};
            }
            return std::string(text.substr(first, text.find_last_not_of(" \t\r\n") - first + 1));
        }

        std::string upper(std::string text)
        {
            std::ranges::transform(text, text.begin(), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
            return text;
        }

        bool fileNameIs(const fs::path& file, std::string_view name)
        {
            return upper(toUtf8(file.filename())) == name;
        }

        std::string todayUtc()
        {
            const auto days = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
            return std::format("{:%F}", std::chrono::year_month_day(days));
        }

        // A permit expiry as yyyy-MM-dd. Permits write it that way or, in the earlier layout, as
        // yyyyMMdd (the KHOA 2020 test data). Empty when it is neither.
        std::string isoDate(std::string_view text)
        {
            const auto digits = [](std::string_view s) { return std::ranges::all_of(s, [](char c) { return c >= '0' && c <= '9'; }); };
            if (text.size() >= 10 && text[4] == '-' && text[7] == '-' && digits(text.substr(0, 4)) && digits(text.substr(5, 2)) && digits(text.substr(8, 2)))
            {
                return std::string(text.substr(0, 10));
            }
            if (text.size() == 8 && digits(text))
            {
                return std::format("{}-{}-{}", text.substr(0, 4), text.substr(4, 2), text.substr(6, 2));
            }
            return {};
        }

        Bytes readFile(const fs::path& file)
        {
            std::ifstream in(file, std::ios::binary);
            if (!in)
            {
                throw ProtectionError(std::format("cannot open {}", toUtf8(file)));
            }
            Bytes bytes{ std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
            if (in.bad())
            {
                throw ProtectionError(std::format("cannot read {}", toUtf8(file)));
            }
            return bytes;
        }

        void writeFile(const fs::path& file, std::span<const std::uint8_t> bytes)
        {
            fs::create_directories(file.parent_path());
            std::ofstream out(file, std::ios::binary | std::ios::trunc);
            out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            if (!out)
            {
                throw ProtectionError(std::format("cannot write {}", toUtf8(file)));
            }
        }

        // A ZIP entry name as a relative path, or nothing when it would leave the folder it is
        // extracted into.
        std::optional<fs::path> safeRelative(std::string_view entryName)
        {
            const fs::path path = fromUtf8(entryName).lexically_normal();
            if (path.empty() || path.is_absolute() || path.has_root_name() || path.has_root_directory())
            {
                return std::nullopt;
            }
            if (std::ranges::any_of(path, [](const fs::path& part) { return part == ".."; }))
            {
                return std::nullopt;
            }
            return path;
        }

        // A folder made for one import and removed with everything in it when the import ends.
        class StagingFolder final
        {
        public:
            explicit StagingFolder(const fs::path& parent)
            {
                std::array<std::uint8_t, 8> random{};
                if (RAND_bytes(random.data(), static_cast<int>(random.size())) != 1)
                {
                    throw ProtectionError("no random name for the staging folder");
                }
                path_ = (parent.empty() ? fs::temp_directory_path() : parent) / ("opens100-protected-" + toHex(random));
                fs::create_directories(path_);
            }
            StagingFolder(const StagingFolder&) = delete;
            StagingFolder& operator=(const StagingFolder&) = delete;
            ~StagingFolder()
            {
                std::error_code ignored;
                fs::remove_all(path_, ignored);
            }

            [[nodiscard]] const fs::path& path() const noexcept { return path_; }

        private:
            fs::path path_;
        };

        enum class SourceKind
        {
            Folder,
            Zip,
            File,
        };

        // Fills the staging folder: decrypts what has a permit, copies the rest.
        class Stager final
        {
        public:
            Stager(const PermitFile& permits, std::string hwId, std::string today, const fs::path& staging, ProtectedImportReport& report)
                : permits_(permits), hwId_(std::move(hwId)), today_(std::move(today)), staging_(staging), report_(report)
            {
            }

            // A file whose content is at hand (a ZIP entry, a single file).
            void stage(const fs::path& relative, Bytes content)
            {
                const auto name = toUtf8(relative.filename());
                if (const DatasetPermit* permit = permits_.find(name))
                {
                    decrypt(relative, *permit, content);
                }
                else
                {
                    writeFile(staging_ / relative, content);
                }
            }

            // A file on disk: only read into memory when it has to be decrypted.
            void stage(const fs::path& relative, const fs::path& file)
            {
                const auto name = toUtf8(relative.filename());
                if (const DatasetPermit* permit = permits_.find(name))
                {
                    decrypt(relative, *permit, readFile(file));
                }
                else
                {
                    fs::create_directories((staging_ / relative).parent_path());
                    fs::copy_file(file, staging_ / relative, fs::copy_options::overwrite_existing);
                }
            }

            void reportUnusedPermits()
            {
                for (const auto& permit : permits_.permits)
                {
                    if (!used_.contains(&permit))
                    {
                        report_.warnings.push_back(std::format("PERMIT.XML grants {}, which the source does not hold", permit.fileName));
                    }
                }
            }

            [[nodiscard]] const std::vector<fs::path>& decrypted() const noexcept { return decrypted_; }

        private:
            void decrypt(const fs::path& relative, const DatasetPermit& permit, const Bytes& encrypted)
            {
                used_.insert(&permit);
                DecryptionResult result{ .file = toGenericUtf8(relative) };
                try
                {
                    const auto key = decryptDataKey(permit.encryptedKey, hwId_);
                    const auto file = decryptFile(encrypted, key);
                    // The staged file keeps the protected file's name, which is the name the
                    // exchange set catalogue lists.
                    writeFile(staging_ / relative, file.content);
                    decrypted_.push_back(staging_ / relative);
                    result.decrypted = true;
                }
                catch (const ProtectionError& error)
                {
                    result.message = error.what();
                }
                report_.files.push_back(std::move(result));

                if (const auto expiry = isoDate(permit.expiry); !expiry.empty() && expiry < today_)
                {
                    report_.expiredPermits.push_back({ permit.fileName, permit.expiry });
                }
            }

            const PermitFile& permits_;
            std::string hwId_;
            std::string today_;
            fs::path staging_;
            ProtectedImportReport& report_;
            std::set<const DatasetPermit*> used_;
            std::vector<fs::path> decrypted_;
        };

        std::optional<fs::path> findFile(const fs::path& folder, std::string_view upperName)
        {
            std::optional<fs::path> best;
            std::error_code error;
            for (auto it = fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied, error);
                 !error && it != fs::recursive_directory_iterator(); it.increment(error))
            {
                if (it->is_regular_file(error) && fileNameIs(it->path(), upperName))
                {
                    // The shallowest one: an exchange set's own catalogue, not one nested in it.
                    const auto depth = std::distance(it->path().begin(), it->path().end());
                    if (!best || depth < std::distance(best->begin(), best->end()))
                    {
                        best = it->path();
                    }
                }
            }
            return best;
        }

        std::optional<std::size_t> findEntry(const ZipArchive& archive, std::string_view upperName)
        {
            std::optional<std::size_t> best;
            const auto& entries = archive.entries();
            for (std::size_t i = 0; i < entries.size(); ++i)
            {
                if (entries[i].isDirectory() || !fileNameIs(fromUtf8(entries[i].name), upperName))
                {
                    continue;
                }
                if (!best || std::ranges::count(entries[i].name, '/') < std::ranges::count(entries[*best].name, '/'))
                {
                    best = i;
                }
            }
            return best;
        }
    }

    std::size_t ProtectedImportReport::decryptedCount() const noexcept
    {
        return static_cast<std::size_t>(std::ranges::count(files, true, &DecryptionResult::decrypted));
    }

    ResolvedCredentials resolveCredentials(const ProtectedImportRequest& request, const PermitFile& permits)
    {
        ResolvedCredentials resolved;

        const std::string given = trimmed(request.userPermit);
        const std::string inPermitFile = trimmed(permits.userPermit);
        if (!given.empty() && !inPermitFile.empty() && upper(given) != upper(inPermitFile))
        {
            throw ProtectionError("the USERPERMIT given is not the one PERMIT.XML was issued for");
        }
        if (const std::string text = given.empty() ? inPermitFile : given; !text.empty())
        {
            resolved.userPermit = UserPermit::parse(text);
        }

        const std::string manufacturerId = trimmed(request.manufacturerId);
        if (!manufacturerId.empty() && resolved.userPermit && upper(manufacturerId) != upper(resolved.userPermit->manufacturerId))
        {
            throw ProtectionError(std::format("the M_ID given ({}) is not the user permit's ({})",
                manufacturerId, resolved.userPermit->manufacturerId));
        }

        const std::string hwId = trimmed(request.hwId);
        const std::string manufacturerKey = trimmed(request.manufacturerKey);
        std::optional<std::string> derived;
        if (!manufacturerKey.empty() && resolved.userPermit)
        {
            derived = decryptHwId(*resolved.userPermit, manufacturerKey);
        }

        if (!hwId.empty())
        {
            resolved.hwId = normaliseKey(hwId, "HW_ID");
            if (derived && *derived != resolved.hwId)
            {
                throw ProtectionError("the HW_ID given is not the one the user permit was issued for (checked with M_KEY)");
            }
        }
        else if (derived)
        {
            resolved.hwId = *derived;
        }
        else
        {
            throw ProtectionError(manufacturerKey.empty()
                ? "a HW_ID is needed: give HW_ID, or M_KEY with a user permit"
                : "a HW_ID is needed: M_KEY was given but there is no user permit to decrypt it from");
        }
        return resolved;
    }

    ProtectedImportReport importProtected(Importer& importer, const ProtectedImportRequest& request, const ProtectedImportOptions& options)
    {
        std::error_code error;
        if (request.source.empty() || !fs::exists(request.source, error))
        {
            throw ProtectionError(std::format("the source {} does not exist", toUtf8(request.source)));
        }

        SourceKind kind = SourceKind::File;
        fs::path folder;
        if (fs::is_directory(request.source, error))
        {
            kind = SourceKind::Folder;
            folder = request.source;
        }
        else if (fileNameIs(request.source, "CATALOG.XML"))
        {
            kind = SourceKind::Folder;
            folder = request.source.parent_path();
        }
        else if (upper(toUtf8(request.source.extension())) == ".ZIP")
        {
            kind = SourceKind::Zip;
        }

        // A .zip is read once and kept for the whole import.
        Bytes zipBytes;
        std::optional<ZipArchive> archive;
        if (kind == SourceKind::Zip)
        {
            zipBytes = readFile(request.source);
            try
            {
                archive.emplace(zipBytes);
            }
            catch (const ZipError& zipError)
            {
                throw ProtectionError(std::format("{} cannot be read: {}", toUtf8(request.source.filename()), zipError.what()));
            }
        }

        PermitFile permits;
        if (!request.permitFile.empty())
        {
            permits = PermitFile::load(request.permitFile);
        }
        else if (kind == SourceKind::Folder)
        {
            const auto found = findFile(folder, "PERMIT.XML");
            if (!found)
            {
                throw ProtectionError("no PERMIT.XML was given and the exchange set holds none");
            }
            permits = PermitFile::load(*found);
        }
        else if (kind == SourceKind::Zip)
        {
            const auto found = findEntry(*archive, "PERMIT.XML");
            if (!found)
            {
                throw ProtectionError("no PERMIT.XML was given and the archive holds none");
            }
            const auto xml = archive->read(archive->entries()[*found]);
            permits = PermitFile::parse(std::string_view(reinterpret_cast<const char*>(xml.data()), xml.size()));
        }
        else
        {
            throw ProtectionError("a single protected file needs its PERMIT.XML");
        }
        if (permits.permits.empty())
        {
            throw ProtectionError("PERMIT.XML grants no files");
        }

        const ResolvedCredentials credentials = resolveCredentials(request, permits);

        ProtectedImportReport report;
        report.hwId = credentials.hwId;
        report.dataServerName = permits.dataServerName;

        const StagingFolder staging(options.stagingParent);
        Stager stager(permits, credentials.hwId, options.today.empty() ? todayUtc() : options.today, staging.path(), report);

        switch (kind)
        {
        case SourceKind::Folder:
            for (auto it = fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied, error);
                 !error && it != fs::recursive_directory_iterator(); it.increment(error))
            {
                std::error_code fileError;
                if (it->is_regular_file(fileError))
                {
                    stager.stage(it->path().lexically_relative(folder), it->path());
                }
            }
            if (error)
            {
                throw ProtectionError(std::format("cannot list {}: {}", toUtf8(folder), error.message()));
            }
            break;
        case SourceKind::Zip:
            for (const auto& entry : archive->entries())
            {
                if (entry.isDirectory())
                {
                    continue;
                }
                const auto relative = safeRelative(entry.name);
                if (!relative)
                {
                    report.warnings.push_back(std::format("{} in the archive points outside it and was skipped", entry.name));
                    continue;
                }
                try
                {
                    stager.stage(*relative, archive->read(entry));
                }
                catch (const ZipError& zipError)
                {
                    report.warnings.push_back(std::format("{} in the archive cannot be read: {}", entry.name, zipError.what()));
                }
            }
            break;
        case SourceKind::File:
            stager.stage(request.source.filename(), readFile(request.source));
            break;
        }
        stager.reportUnusedPermits();

        if (const auto catalogue = findFile(staging.path(), "CATALOG.XML"))
        {
            report.import = importer.importExchangeSet(*catalogue, options.progress);
        }
        else
        {
            // Loose dataset files: the decrypted ones, base datasets before their updates.
            auto files = stager.decrypted();
            std::ranges::sort(files);
            for (const auto& file : files)
            {
                report.import.results.push_back(importer.importDataset(file));
            }
        }

        // The staging folder is about to go: name each result by where it came from instead.
        const fs::path origin = kind == SourceKind::Folder ? folder
            : kind == SourceKind::Zip ? request.source
            : request.source.parent_path();
        for (auto& result : report.import.results)
        {
            const fs::path relative = result.source.lexically_relative(staging.path());
            if (!relative.empty() && *relative.begin() != "..")
            {
                result.source = origin / relative;
            }
        }
        return report;
    }
}
