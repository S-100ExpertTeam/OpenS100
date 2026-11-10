#include "Importer.h"

#include "ResourceStore.h"
#include "Sha256.h"
#include "SystemDatabase.h"
#include "Utf8Path.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <format>
#include <tuple>

namespace s100::sdb
{
    namespace fs = std::filesystem;

    std::string_view toString(ImportOutcome outcome) noexcept
    {
        switch (outcome)
        {
        case ImportOutcome::Added:       return "added";
        case ImportOutcome::Replaced:    return "replaced";
        case ImportOutcome::UpdateAdded: return "update added";
        case ImportOutcome::Cancelled:   return "cancelled";
        case ImportOutcome::Removed:     return "removed";
        case ImportOutcome::Unchanged:   return "unchanged";
        case ImportOutcome::Rejected:    return "rejected";
        }
        return "rejected";
    }

    std::size_t ImportReport::count(ImportOutcome outcome) const noexcept
    {
        return static_cast<std::size_t>(std::ranges::count(results, outcome, &ImportResult::outcome));
    }

    DatasetDecision decideDataset(const InspectedDataset& incoming, const std::optional<DatasetRecord>& installed,
        int highestUpdate, bool sameContent)
    {
        if (incoming.isUpdate && incoming.edition == 0)
        {
            if (!installed)
            {
                return { DatasetAction::Reject, "a cancellation of a dataset that is not installed" };
            }
            if (installed->status == DatasetStatus::Cancelled)
            {
                return { DatasetAction::Unchanged, "already cancelled" };
            }
            return { DatasetAction::Cancel, "cancelled by its producer" };
        }

        if (!installed)
        {
            if (incoming.isUpdate)
            {
                return { DatasetAction::Reject, std::format("update {} of a dataset that is not installed", incoming.updateNumber) };
            }
            return { DatasetAction::Add, "new dataset" };
        }

        const bool editionsKnown = incoming.edition && installed->edition;
        if (!incoming.isUpdate)
        {
            if (editionsKnown && *incoming.edition > *installed->edition)
            {
                return { DatasetAction::Replace, std::format("new edition {}", *incoming.edition) };
            }
            if (editionsKnown && *incoming.edition < *installed->edition)
            {
                return { DatasetAction::Reject, std::format("edition {} is older than the installed edition {}",
                    *incoming.edition, *installed->edition) };
            }
            if (sameContent)
            {
                return { DatasetAction::Unchanged, "already installed" };
            }
            if (installed->status == DatasetStatus::Cancelled)
            {
                return { DatasetAction::Reject, "the dataset was cancelled; only a new edition reinstates it" };
            }
            if (incoming.updateNumber < highestUpdate)
            {
                return { DatasetAction::Reject, std::format("a re-issue at update {} is older than the installed update {}",
                    incoming.updateNumber, highestUpdate) };
            }
            return { DatasetAction::Replace, "re-issue" };
        }

        if (installed->status == DatasetStatus::Cancelled)
        {
            return { DatasetAction::Reject, "the dataset was cancelled" };
        }
        if (editionsKnown && *incoming.edition != *installed->edition)
        {
            return { DatasetAction::Reject, std::format("update for edition {}, but edition {} is installed",
                *incoming.edition, *installed->edition) };
        }
        if (incoming.updateNumber <= highestUpdate)
        {
            return { DatasetAction::Unchanged, std::format("update {} already installed", incoming.updateNumber) };
        }
        if (incoming.updateNumber > highestUpdate + 1)
        {
            return { DatasetAction::Reject, std::format("update {} needs update {} first", incoming.updateNumber, highestUpdate + 1) };
        }
        return { DatasetAction::AddUpdate, std::format("update {}", incoming.updateNumber) };
    }

    // --- staged file changes -------------------------------------------------------------------

    namespace
    {
        std::string parentOf(std::string_view relative)
        {
            return toGenericUtf8(fromUtf8(relative).parent_path());
        }

        ImportResult result(const fs::path& source, ImportOutcome outcome, RecordId id, std::string message)
        {
            return ImportResult{ .source = source, .outcome = outcome, .id = id, .message = std::move(message) };
        }

        ImportResult rejected(const fs::path& source, std::string message, RecordId id = 0)
        {
            return result(source, ImportOutcome::Rejected, id, std::move(message));
        }

        // SHA-256 over every file of a folder, by relative path, so a changed rule file or symbol
        // changes the catalogue's fingerprint, not only a changed portrayal_catalogue.xml.
        std::string treeHash(const fs::path& folder)
        {
            std::vector<std::pair<std::string, fs::path>> files;
            for (const auto& entry : fs::recursive_directory_iterator(folder))
            {
                if (entry.is_regular_file())
                {
                    files.emplace_back(toGenericUtf8(entry.path().lexically_relative(folder)), entry.path());
                }
            }
            std::ranges::sort(files);
            Sha256 hash;
            for (const auto& [name, file] : files)
            {
                const std::string line = name + "\n" + sha256File(file) + "\n";
                hash.update(std::span(reinterpret_cast<const std::uint8_t*>(line.data()), line.size()));
            }
            return Sha256::toHex(hash.finish());
        }

        std::optional<fs::path> findPortrayalCatalogueXml(const fs::path& folder)
        {
            for (const auto& entry : fs::directory_iterator(folder))
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }
                std::string name = toUtf8(entry.path().filename());
                std::ranges::transform(name, name.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                if (name == "portrayal_catalogue.xml")
                {
                    return entry.path();
                }
            }
            return std::nullopt;
        }

        std::vector<DataCoverageRecord> coverageRecords(const std::vector<InspectedCoverage>& coverages)
        {
            std::vector<DataCoverageRecord> records;
            for (const InspectedCoverage& coverage : coverages)
            {
                const auto bounds = boundsOf(coverage.rings);
                if (!bounds)
                {
                    continue;   // no geometry: nothing to select it by
                }
                records.push_back(DataCoverageRecord{
                    .optimumDisplayScale = coverage.optimumDisplayScale,
                    .minimumDisplayScale = coverage.minimumDisplayScale,
                    .maximumDisplayScale = coverage.maximumDisplayScale,
                    .bounds = *bounds,
                    .polygon = encodeWkbPolygon(coverage.rings),
                });
            }
            return records;
        }

        std::optional<BoundingBox> datasetBounds(const InspectedDataset& dataset)
        {
            if (dataset.bounds)
            {
                return dataset.bounds;
            }
            std::optional<BoundingBox> bounds;
            for (const InspectedCoverage& coverage : dataset.coverages)
            {
                if (const auto box = boundsOf(coverage.rings))
                {
                    bounds = bounds ? merge(*bounds, *box) : *box;
                }
            }
            return bounds;
        }

        // What the exchange set catalogue says fills in what the dataset itself leaves out.
        void fillFromCatalogue(InspectedDataset& dataset, const InspectedExchangeSet::Dataset& entry)
        {
            if (dataset.product.empty()) dataset.product = entry.product;
            if (dataset.productEdition.empty()) dataset.productEdition = entry.productEdition;
            if (!dataset.edition) dataset.edition = entry.edition;
            if (dataset.issueDate.empty()) dataset.issueDate = entry.issueDate;
            if (!dataset.bounds) dataset.bounds = entry.bounds;
            if (dataset.coverages.empty()) dataset.coverages = entry.coverages;
        }
    }

    // Copies made into a staging folder inside the store during the transaction, and the files
    // to delete, applied only after it has committed. Whatever is left in the staging folder is
    // deleted with this object.
    class Importer::Changes final
    {
    public:
        explicit Changes(ResourceStore& store) : store_(&store), staging_(newStagingDirectory()) {}
        Changes(const Changes&) = delete;
        Changes& operator=(const Changes&) = delete;

        ~Changes()
        {
            try
            {
                (void)store_->remove(staging_);
            }
            catch (...)
            {
                // Leftover staging files are harmless; the next import uses another folder.
            }
        }

        // Returns the SHA-256 of the staged copy.
        std::string stageFile(const fs::path& source, std::string finalRelative)
        {
            const std::string staged = nextStagingPath();
            const std::string digest = store_->copyFile(source, staged);
            installs_.emplace_back(staged, std::move(finalRelative));
            return digest;
        }

        void stageDirectory(const fs::path& source, std::string finalRelative)
        {
            const std::string staged = nextStagingPath();
            (void)store_->copyDirectory(source, staged);
            installs_.emplace_back(staged, std::move(finalRelative));
        }

        void removeAfterCommit(std::string relative)
        {
            removals_.push_back(std::move(relative));
        }

        // Removals first, so a replaced dataset's folder is cleared before its new files arrive.
        // Failures are reported, not thrown: the database already records the new state.
        std::vector<std::string> apply()
        {
            std::vector<std::string> problems;
            for (const std::string& relative : removals_)
            {
                try
                {
                    (void)store_->remove(relative);
                }
                catch (const std::exception& e)
                {
                    problems.push_back(std::format("cannot remove {}: {}", relative, e.what()));
                }
            }
            for (const auto& [staged, target] : installs_)
            {
                try
                {
                    store_->move(staged, target);
                }
                catch (const std::exception& e)
                {
                    problems.push_back(std::format("cannot install {}: {}", target, e.what()));
                }
            }
            removals_.clear();
            installs_.clear();
            return problems;
        }

    private:
        static std::string newStagingDirectory()
        {
            static std::atomic<unsigned> counter = 0;
            const auto ticks = std::chrono::steady_clock::now().time_since_epoch().count();
            return std::format("STAGING/{}-{}", ticks, ++counter);
        }

        std::string nextStagingPath()
        {
            return std::format("{}/{}", staging_, installs_.size());
        }

        ResourceStore* store_;
        std::string staging_;
        std::vector<std::pair<std::string, std::string>> installs_;
        std::vector<std::string> removals_;
    };

    struct Importer::SupportFileInput
    {
        fs::path file;
        std::string product;
        std::string purpose;
        std::optional<int> edition;
        std::string format;
        std::string issueDate;
    };

    // --- Importer ------------------------------------------------------------------------------

    std::string Importer::utcNow()
    {
        return std::format("{:%Y-%m-%dT%H:%M:%SZ}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
    }

    Importer::Importer(SystemDatabase& database, ResourceStore& store, Readers readers, Clock clock)
        : database_(&database), store_(&store), readers_(std::move(readers)), clock_(std::move(clock))
    {
    }

    const DatasetInspector* Importer::inspectorFor(const fs::path& file) const
    {
        const auto found = std::ranges::find_if(readers_.datasets,
            [&](const DatasetInspector* inspector) { return inspector && inspector->accepts(file); });
        return found == readers_.datasets.end() ? nullptr : *found;
    }

    ImportResult Importer::importFeatureCatalogue(const fs::path& xml)
    {
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        ImportResult imported = installFeatureCatalogue(xml, changes);
        transaction.commit();
        for (const std::string& problem : changes.apply())
        {
            imported.message += "; " + problem;
        }
        return imported;
    }

    ImportResult Importer::importPortrayalCatalogue(const fs::path& location)
    {
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        ImportResult imported = installPortrayalCatalogue(location, changes);
        transaction.commit();
        for (const std::string& problem : changes.apply())
        {
            imported.message += "; " + problem;
        }
        return imported;
    }

    ImportResult Importer::importDataset(const fs::path& file)
    {
        const DatasetInspector* inspector = inspectorFor(file);
        if (!inspector)
        {
            return rejected(file, "no reader for this kind of file");
        }
        auto dataset = inspector->inspect(file);
        if (!dataset)
        {
            return rejected(file, "cannot be read as a dataset");
        }

        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        ImportResult imported = installDataset(file, std::move(*dataset), std::nullopt, changes);
        transaction.commit();
        for (const std::string& problem : changes.apply())
        {
            imported.message += "; " + problem;
        }
        return imported;
    }

    ImportResult Importer::importSupportFile(const fs::path& file, std::string_view product, std::string_view purpose)
    {
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        ImportResult imported = installSupportFile(SupportFileInput{
            .file = file, .product = std::string(product), .purpose = std::string(purpose) }, changes);
        transaction.commit();
        for (const std::string& problem : changes.apply())
        {
            imported.message += "; " + problem;
        }
        return imported;
    }

    ImportReport Importer::importExchangeSet(const fs::path& catalogXml, const Progress& progress)
    {
        ImportReport report;
        if (!readers_.exchangeSets)
        {
            report.results.push_back(rejected(catalogXml, "no exchange set reader"));
            return report;
        }
        auto set = readers_.exchangeSets->read(catalogXml);
        if (!set)
        {
            report.results.push_back(rejected(catalogXml, "cannot be read as an exchange set catalogue"));
            return report;
        }

        // Read every dataset before the transaction: parsing is the slow part and holds no lock.
        struct Pending
        {
            fs::path file;
            std::optional<InspectedDataset> dataset;
            std::string problem;
        };
        std::vector<Pending> pending;
        pending.reserve(set->datasets.size());
        for (const auto& entry : set->datasets)
        {
            Pending item{ .file = entry.file };
            std::error_code error;
            const DatasetInspector* inspector = inspectorFor(entry.file);
            if (!fs::is_regular_file(entry.file, error))
            {
                item.problem = "listed in the catalogue but not found";
            }
            else if (!inspector)
            {
                item.problem = "no reader for this kind of file";
            }
            else if (auto dataset = inspector->inspect(entry.file))
            {
                fillFromCatalogue(*dataset, entry);
                item.dataset = std::move(dataset);
            }
            else
            {
                item.problem = "cannot be read as a dataset";
            }
            pending.push_back(std::move(item));
            if (progress)
            {
                progress(pending.size(), set->datasets.size());
            }
        }
        // Base datasets before their updates, and updates in order.
        std::ranges::stable_sort(pending, {}, [](const Pending& item) {
            return item.dataset
                ? std::make_tuple(0, item.dataset->product, item.dataset->name, item.dataset->isUpdate, item.dataset->updateNumber)
                : std::make_tuple(1, std::string(), std::string(), false, 0);
        });

        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        const RecordId exchangeSetId = database_->exchangeSets().add(ExchangeSetRecord{
            .sourcePath = toUtf8(catalogXml),
            .identifier = set->identifier,
            .issueDate = set->issueDate,
            .importedAt = clock_(),
        });
        report.exchangeSetId = exchangeSetId;

        for (const auto& catalogue : set->catalogues)
        {
            report.results.push_back(catalogue.kind == CatalogueKind::Feature
                ? installFeatureCatalogue(catalogue.file, changes)
                : installPortrayalCatalogue(catalogue.file, changes));
        }
        // Support files before the datasets, so the datasets can be linked to them.
        for (const auto& supportFile : set->supportFiles)
        {
            report.results.push_back(installSupportFile(SupportFileInput{
                .file = supportFile.file,
                .product = supportFile.product,
                .purpose = supportFile.purpose,
                .edition = supportFile.edition,
                .format = supportFile.format,
                .issueDate = supportFile.issueDate,
            }, changes));
        }
        for (Pending& item : pending)
        {
            report.results.push_back(item.dataset
                ? installDataset(item.file, std::move(*item.dataset), exchangeSetId, changes)
                : rejected(item.file, item.problem));
        }

        transaction.commit();
        report.installProblems = changes.apply();
        return report;
    }

    ImportResult Importer::installFeatureCatalogue(const fs::path& xml, Changes& changes)
    {
        if (!readers_.catalogues)
        {
            return rejected(xml, "no catalogue reader");
        }
        try
        {
            const auto catalogue = readers_.catalogues->inspectFeatureCatalogue(xml);
            if (!catalogue || catalogue->product.empty() || catalogue->version.empty())
            {
                return rejected(xml, "not a feature catalogue");
            }
            auto& catalogues = database_->featureCatalogues();
            const std::string relative = ResourceStore::featureCataloguePath(catalogue->product, catalogue->version, toUtf8(xml.filename()));
            const std::string digest = sha256File(xml);
            const auto installed = catalogues.find(catalogue->product, catalogue->version);
            if (installed && installed->sha256 == digest)
            {
                return result(xml, ImportOutcome::Unchanged, installed->id, "already installed");
            }

            (void)changes.stageFile(xml, relative);
            CatalogueRecord record{
                .product = catalogue->product,
                .version = catalogue->version,
                .path = relative,
                .sha256 = digest,
                .issueDate = catalogue->issueDate,
                .importedAt = clock_(),
            };
            if (installed)
            {
                record.id = installed->id;
                (void)catalogues.update(record);
                if (installed->path != relative)
                {
                    changes.removeAfterCommit(installed->path);
                }
                return result(xml, ImportOutcome::Replaced, record.id, "changed catalogue of the same version");
            }
            const RecordId id = catalogues.add(record);
            return result(xml, ImportOutcome::Added, id, std::format("{} {}", catalogue->product, catalogue->version));
        }
        catch (const fs::filesystem_error& e)
        {
            return rejected(xml, e.what());
        }
    }

    ImportResult Importer::installPortrayalCatalogue(const fs::path& location, Changes& changes)
    {
        if (!readers_.catalogues)
        {
            return rejected(location, "no catalogue reader");
        }
        try
        {
            const fs::path folder = fs::is_directory(location) ? location : location.parent_path();
            const auto xml = findPortrayalCatalogueXml(folder);
            if (!xml)
            {
                return rejected(location, "no portrayal_catalogue.xml in the folder");
            }
            const auto catalogue = readers_.catalogues->inspectPortrayalCatalogue(*xml);
            if (!catalogue || catalogue->product.empty() || catalogue->version.empty())
            {
                return rejected(location, "not a portrayal catalogue");
            }
            auto& catalogues = database_->portrayalCatalogues();
            const std::string directory = ResourceStore::portrayalCatalogueDirectory(catalogue->product, catalogue->version);
            const std::string relative = directory + "/" + toUtf8(xml->filename());
            const std::string digest = treeHash(folder);
            const auto installed = catalogues.find(catalogue->product, catalogue->version);
            if (installed && installed->sha256 == digest)
            {
                return result(location, ImportOutcome::Unchanged, installed->id, "already installed");
            }

            changes.stageDirectory(folder, directory);
            CatalogueRecord record{
                .product = catalogue->product,
                .version = catalogue->version,
                .path = relative,
                .sha256 = digest,
                .issueDate = catalogue->issueDate,
                .importedAt = clock_(),
            };
            if (installed)
            {
                record.id = installed->id;
                (void)catalogues.update(record);
                return result(location, ImportOutcome::Replaced, record.id, "changed catalogue of the same version");
            }
            const RecordId id = catalogues.add(record);
            return result(location, ImportOutcome::Added, id, std::format("{} {}", catalogue->product, catalogue->version));
        }
        catch (const fs::filesystem_error& e)
        {
            return rejected(location, e.what());
        }
    }

    ImportResult Importer::installDataset(const fs::path& file, InspectedDataset dataset,
        std::optional<RecordId> exchangeSetId, Changes& changes)
    {
        if (dataset.name.empty())
        {
            dataset.name = toUtf8(file.stem());
        }
        if (dataset.product.empty())
        {
            return rejected(file, "the dataset does not name its product");
        }

        try
        {
            auto& datasets = database_->datasets();
            const auto installed = datasets.find(dataset.product, dataset.name);
            int highestUpdate = 0;
            bool sameContent = false;
            if (installed)
            {
                highestUpdate = installed->updateNumber;
                for (const DatasetUpdateRecord& update : datasets.updates(installed->id))
                {
                    highestUpdate = std::max(highestUpdate, update.updateNumber);
                }
                if (!dataset.isUpdate && installed->edition == dataset.edition && store_->exists(installed->path))
                {
                    sameContent = sha256File(store_->absolute(installed->path)) == sha256File(file);
                }
            }

            const DatasetDecision decision = decideDataset(dataset, installed, highestUpdate, sameContent);
            const std::string relative = ResourceStore::datasetFilePath(dataset.product, dataset.name, toUtf8(file.filename()));
            switch (decision.action)
            {
            case DatasetAction::Reject:
                return rejected(file, decision.reason, installed ? installed->id : 0);

            case DatasetAction::Unchanged:
                return result(file, ImportOutcome::Unchanged, installed->id, decision.reason);

            case DatasetAction::Cancel:
            {
                DatasetRecord record = *installed;
                record.status = DatasetStatus::Cancelled;
                record.importedAt = clock_();
                (void)datasets.update(record);
                return result(file, ImportOutcome::Cancelled, record.id, decision.reason);
            }

            case DatasetAction::AddUpdate:
                (void)changes.stageFile(file, relative);
                datasets.addUpdate(DatasetUpdateRecord{
                    .datasetId = installed->id,
                    .updateNumber = dataset.updateNumber,
                    .path = relative,
                    .issueDate = dataset.issueDate,
                    .applied = false,
                });
                return result(file, ImportOutcome::UpdateAdded, installed->id, decision.reason);

            case DatasetAction::Add:
            case DatasetAction::Replace:
                break;
            }

            (void)changes.stageFile(file, relative);
            DatasetRecord record{
                .name = dataset.name,
                .product = dataset.product,
                .productEdition = dataset.productEdition,
                .format = dataset.format,
                .edition = dataset.edition,
                .updateNumber = dataset.updateNumber,
                .issueDate = dataset.issueDate,
                .path = relative,
                .bounds = datasetBounds(dataset),
                .status = DatasetStatus::Active,
                .exchangeSetId = exchangeSetId,
                .importedAt = clock_(),
            };
            RecordId id = 0;
            if (decision.action == DatasetAction::Add)
            {
                id = datasets.add(record);
            }
            else
            {
                id = installed->id;
                record.id = id;
                if (!exchangeSetId)
                {
                    record.exchangeSetId = installed->exchangeSetId;
                }
                (void)datasets.removeUpdates(id);
                for (const RecordId supportFileId : datasets.supportFileIds(id))
                {
                    (void)datasets.unlinkSupportFile(id, supportFileId);
                }
                (void)datasets.update(record);
                // The old edition and its updates go; the new base file arrives after.
                changes.removeAfterCommit(parentOf(installed->path));
            }
            const auto coverages = coverageRecords(dataset.coverages);
            (void)database_->coverages().replace(id, coverages);
            linkSupportFiles(id, dataset);

            std::string message = decision.reason;
            if (coverages.empty())
            {
                message += "; no DataCoverage, so it is never selected for display";
            }
            return result(file, decision.action == DatasetAction::Add ? ImportOutcome::Added : ImportOutcome::Replaced, id, message);
        }
        catch (const fs::filesystem_error& e)
        {
            return rejected(file, e.what());
        }
    }

    void Importer::linkSupportFiles(RecordId datasetId, const InspectedDataset& dataset)
    {
        auto& supportFiles = database_->supportFiles();
        for (const std::string& name : dataset.supportFileReferences)
        {
            if (const auto supportFile = supportFiles.findByPath(ResourceStore::supportFilePath(dataset.product, name)))
            {
                database_->datasets().linkSupportFile(datasetId, supportFile->id);
            }
        }
    }

    ImportResult Importer::installSupportFile(const SupportFileInput& input, Changes& changes)
    {
        try
        {
            const std::string name = toUtf8(input.file.filename());
            const std::string product = input.product.empty() ? "S-100" : input.product;
            const std::string relative = ResourceStore::supportFilePath(product, name);
            auto& supportFiles = database_->supportFiles();
            const auto installed = supportFiles.findByPath(relative);

            if (input.purpose == "deletion")
            {
                if (!installed)
                {
                    return result(input.file, ImportOutcome::Unchanged, 0, "deletion of a support file that is not installed");
                }
                (void)supportFiles.remove(installed->id);
                changes.removeAfterCommit(relative);
                return result(input.file, ImportOutcome::Removed, installed->id, "deleted by its producer");
            }

            const std::string digest = sha256File(input.file);
            if (installed && installed->sha256 == digest)
            {
                return result(input.file, ImportOutcome::Unchanged, installed->id, "already installed");
            }
            (void)changes.stageFile(input.file, relative);
            SupportFileRecord record{
                .name = name,
                .path = relative,
                .purpose = input.purpose,
                .edition = input.edition,
                .format = input.format,
                .issueDate = input.issueDate,
                .sha256 = digest,
                .importedAt = clock_(),
            };
            if (installed)
            {
                record.id = installed->id;
                (void)supportFiles.update(record);
                return result(input.file, ImportOutcome::Replaced, record.id, "changed support file");
            }
            const RecordId id = supportFiles.add(record);
            return result(input.file, ImportOutcome::Added, id, "new support file");
        }
        catch (const fs::filesystem_error& e)
        {
            return rejected(input.file, e.what());
        }
    }

    bool Importer::removeDataset(RecordId id)
    {
        const auto installed = database_->datasets().find(id);
        if (!installed)
        {
            return false;
        }
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        (void)database_->datasets().remove(id);
        changes.removeAfterCommit(parentOf(installed->path));
        transaction.commit();
        (void)changes.apply();
        return true;
    }

    bool Importer::removeFeatureCatalogue(RecordId id)
    {
        const auto installed = database_->featureCatalogues().find(id);
        if (!installed)
        {
            return false;
        }
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        (void)database_->featureCatalogues().remove(id);
        changes.removeAfterCommit(parentOf(installed->path));
        transaction.commit();
        (void)changes.apply();
        return true;
    }

    bool Importer::removePortrayalCatalogue(RecordId id)
    {
        const auto installed = database_->portrayalCatalogues().find(id);
        if (!installed)
        {
            return false;
        }
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        (void)database_->portrayalCatalogues().remove(id);
        changes.removeAfterCommit(parentOf(installed->path));
        transaction.commit();
        (void)changes.apply();
        return true;
    }

    bool Importer::removeSupportFile(RecordId id)
    {
        const auto installed = database_->supportFiles().find(id);
        if (!installed)
        {
            return false;
        }
        Changes changes(*store_);
        Transaction transaction = database_->transaction();
        (void)database_->supportFiles().remove(id);
        changes.removeAfterCommit(installed->path);
        transaction.commit();
        (void)changes.apply();
        return true;
    }

    bool Importer::removeExchangeSet(RecordId id)
    {
        return database_->exchangeSets().remove(id);
    }
}
