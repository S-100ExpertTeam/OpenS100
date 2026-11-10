#pragma once

#include "Inspection.h"
#include "Records.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace s100::sdb
{
    class ResourceStore;
    class SystemDatabase;

    enum class ImportOutcome
    {
        Added,         // new to the database
        Replaced,      // a new edition, a re-issue, or a changed catalogue or support file
        UpdateAdded,   // an update file (.001, ...) recorded for an installed dataset
        Cancelled,     // a cancellation (edition 0) of an installed dataset
        Removed,       // a support file delivered for deletion
        Unchanged,     // already installed as it is
        Rejected,      // not installed; `message` says why
    };

    [[nodiscard]] std::string_view toString(ImportOutcome outcome) noexcept;

    struct ImportResult
    {
        std::filesystem::path source;
        ImportOutcome outcome = ImportOutcome::Rejected;
        RecordId id = 0;          // the record written or found; 0 when rejected
        std::string message;
    };

    struct ImportReport
    {
        std::optional<RecordId> exchangeSetId;
        std::vector<ImportResult> results;
        // Files that were recorded but could not be moved into place after the transaction
        // committed (a full disk, a file locked by another program). Importing again repairs them.
        std::vector<std::string> installProblems;

        [[nodiscard]] std::size_t count(ImportOutcome outcome) const noexcept;
    };

    // What becomes of a dataset file given what is installed: the edition and update rules of
    // S-100 Part 17 and S-101, kept apart from the file handling so they can be tested alone.
    enum class DatasetAction
    {
        Add,
        Replace,
        AddUpdate,
        Cancel,
        Unchanged,
        Reject,
    };

    struct DatasetDecision
    {
        DatasetAction action = DatasetAction::Reject;
        std::string reason;
    };

    // `installed` is the dataset of the same product and name, if any; `highestUpdate` the
    // highest update number recorded for it; `sameContent` whether the file is byte for byte the
    // installed base file (only consulted for a base file of the installed edition).
    [[nodiscard]] DatasetDecision decideDataset(const InspectedDataset& incoming, const std::optional<DatasetRecord>& installed,
        int highestUpdate, bool sameContent);

    // Installs S-100 resources into the System Database: reads each file through the registered
    // readers, copies it into the managed store and records it, all of one call in one transaction.
    // Files are copied to a staging area first and moved into place once the transaction has
    // committed, so a failed import leaves the installed files as they were.
    class Importer final
    {
    public:
        struct Readers
        {
            std::vector<const DatasetInspector*> datasets;
            const CatalogueInspector* catalogues = nullptr;
            const ExchangeSetReader* exchangeSets = nullptr;
        };

        // ISO 8601 UTC time for the importedAt columns; replaceable for tests.
        using Clock = std::function<std::string()>;
        [[nodiscard]] static std::string utcNow();

        Importer(SystemDatabase& database, ResourceStore& store, Readers readers, Clock clock = &Importer::utcNow);

        ImportResult importFeatureCatalogue(const std::filesystem::path& xml);
        // `location` is the catalogue folder, the portrayal_catalogue.xml in it, or a .zip archive
        // that holds the folder (at any depth); the whole folder is installed.
        ImportResult importPortrayalCatalogue(const std::filesystem::path& location);
        // A base dataset, an update file or a cancellation.
        ImportResult importDataset(const std::filesystem::path& file);
        // `product` is the product the file belongs to ("S-101"), which places it in the store.
        ImportResult importSupportFile(const std::filesystem::path& file, std::string_view product, std::string_view purpose = {});
        // Every dataset, support file and catalogue the exchange set lists. One dataset that
        // cannot be installed does not stop the others; its result says why. `progress` is called
        // after each dataset has been read, with the number read and the number listed.
        // A catalogue that cannot be read is reported as rejected, and the datasets and support
        // files in its folder are imported one by one instead (as importFolder does).
        using Progress = std::function<void(std::size_t done, std::size_t total)>;
        ImportReport importExchangeSet(const std::filesystem::path& catalogXml, const Progress& progress = {});
        // Everything under `folder`: each exchange set (CATALOG.XML) with what it lists, then the
        // datasets and support files no readable catalogue accounts for, one by one. Support files
        // are the files in a SUPPORT_FILES folder; other files are left out. A file met again in
        // the same call (same name, same SHA-256) is imported once: the later copies are reported
        // as unchanged, naming the first. An exchange set whose CATALOG.XML is a copy of one
        // already met is skipped as a whole.
        ImportReport importFolder(const std::filesystem::path& folder, const Progress& progress = {});

        // Whether a registered reader takes files like this one as datasets (from the name).
        [[nodiscard]] bool acceptsDataset(const std::filesystem::path& file) const { return inspectorFor(file) != nullptr; }

        // Removal deletes the record and the files in the store. Each returns false when there
        // was no such record.
        bool removeDataset(RecordId id);
        bool removeFeatureCatalogue(RecordId id);
        bool removePortrayalCatalogue(RecordId id);
        bool removeSupportFile(RecordId id);
        // The exchange set record only; its datasets stay installed.
        bool removeExchangeSet(RecordId id);

    private:
        class Changes;
        class SeenFiles;
        struct SupportFileInput;

        // `seen` (may be null) skips files already met; `fallBackToFiles` imports the folder's
        // files one by one when the catalogue cannot be read.
        ImportReport importExchangeSet(const std::filesystem::path& catalogXml, const Progress& progress, SeenFiles* seen,
            bool fallBackToFiles);
        // Datasets and support files (see importFolder) among `files`, in path order.
        void importFiles(std::vector<std::filesystem::path> files, SeenFiles& seen, ImportReport& report);

        [[nodiscard]] const DatasetInspector* inspectorFor(const std::filesystem::path& file) const;

        ImportResult installFeatureCatalogue(const std::filesystem::path& xml, Changes& changes);
        ImportResult installPortrayalCatalogue(const std::filesystem::path& location, Changes& changes);
        ImportResult installDataset(const std::filesystem::path& file, InspectedDataset dataset,
            std::optional<RecordId> exchangeSetId, Changes& changes);
        ImportResult installSupportFile(const SupportFileInput& input, Changes& changes);
        void linkSupportFiles(RecordId datasetId, const InspectedDataset& dataset);

        SystemDatabase* database_;
        ResourceStore* store_;
        Readers readers_;
        Clock clock_;
    };
}
