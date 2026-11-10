#pragma once

#include "CatalogueRepository.h"
#include "CoverageRepository.h"
#include "DatasetRepository.h"
#include "ExchangeSetRepository.h"
#include "Records.h"
#include "SupportFileRepository.h"
#include "Sqlite.h"

#include <filesystem>
#include <memory>

// The System Database: the local store of every installed S-100 resource (datasets and their
// updates, feature catalogues, portrayal catalogues, support files, imported exchange sets).
// Design: Documents/TechDocs/2026-11-10-system-database.md.
//
// This class owns the SQLite connection and the schema; it knows nothing about file contents.
// Reading S-100 files and copying them into the managed store is the importer's job.
namespace s100::sdb
{
    class SystemDatabase final
    {
    public:
        // Opens the database file, creating it and its parent directories when missing, and
        // migrates the schema to schema::CurrentVersion. Throws DatabaseError when the file cannot
        // be opened or was written by a newer library.
        [[nodiscard]] static SystemDatabase open(const std::filesystem::path& file);
        // A private, empty database for tests and previews.
        [[nodiscard]] static SystemDatabase openInMemory();

        SystemDatabase(SystemDatabase&&) noexcept = default;
        SystemDatabase& operator=(SystemDatabase&&) noexcept = default;
        ~SystemDatabase() = default;

        [[nodiscard]] int schemaVersion() const;

        // Groups several repository calls into one atomic change.
        [[nodiscard]] Transaction transaction() { return Transaction(*connection_); }

        [[nodiscard]] ExchangeSetRepository& exchangeSets() noexcept { return exchangeSets_; }
        [[nodiscard]] CatalogueRepository& featureCatalogues() noexcept { return featureCatalogues_; }
        [[nodiscard]] CatalogueRepository& portrayalCatalogues() noexcept { return portrayalCatalogues_; }
        [[nodiscard]] DatasetRepository& datasets() noexcept { return datasets_; }
        [[nodiscard]] CoverageRepository& coverages() noexcept { return coverages_; }
        [[nodiscard]] SupportFileRepository& supportFiles() noexcept { return supportFiles_; }

        [[nodiscard]] const ExchangeSetRepository& exchangeSets() const noexcept { return exchangeSets_; }
        [[nodiscard]] const CatalogueRepository& featureCatalogues() const noexcept { return featureCatalogues_; }
        [[nodiscard]] const CatalogueRepository& portrayalCatalogues() const noexcept { return portrayalCatalogues_; }
        [[nodiscard]] const DatasetRepository& datasets() const noexcept { return datasets_; }
        [[nodiscard]] const CoverageRepository& coverages() const noexcept { return coverages_; }
        [[nodiscard]] const SupportFileRepository& supportFiles() const noexcept { return supportFiles_; }

    private:
        explicit SystemDatabase(std::unique_ptr<Connection> connection);

        // Heap-allocated so the repositories' pointers stay valid when the database is moved.
        std::unique_ptr<Connection> connection_;
        ExchangeSetRepository exchangeSets_;
        CatalogueRepository featureCatalogues_;
        CatalogueRepository portrayalCatalogues_;
        DatasetRepository datasets_;
        CoverageRepository coverages_;
        SupportFileRepository supportFiles_;
    };
}
