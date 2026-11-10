#pragma once

#include "CoverageModel.h"

#include <cstdint>
#include <filesystem>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

// Reader for S-100 Part 10c HDF5 datasets with regular-grid coverages (opens100-web
// `packages/hdf5/src/read-hdf5.ts`). Layout, as written by S-102, S-104, S-111 and KHOA's tiles:
//
//   /                          product metadata (bounds, productSpecification, ...)
//   /Group_F/featureCode       feature codes in the file
//   /Group_F/<Code>            name, unit, fill value and range per value member
//   /<Code>/<Code>.01          one feature instance: grid origin, spacing, size
//   /<Code>/<Code>.01/Group_001/values   one time step, rows x cols [x depth]
//
// Grids stay on the dataset's horizontalCRS: a UTM one (NOAA's S-102) keeps its eastings and
// northings. Only metadata is read when the file is opened; values are read on demand and the
// last 48 grids are kept. Ungeorectified grids (dataCodingFormat 3) are reported as skipped.
namespace s100::coverage
{
    class Hdf5Coverage final : public CoverageSource
    {
    public:
        struct Skipped
        {
            std::string code;     // a feature code, or an instance path
            std::string reason;
        };

        // Throws std::runtime_error, naming the file and the reason, when it is not a readable
        // S-100 HDF5 dataset with at least one regular-grid coverage.
        [[nodiscard]] static std::unique_ptr<Hdf5Coverage> open(const std::filesystem::path& file);

        Hdf5Coverage(const Hdf5Coverage&) = delete;
        Hdf5Coverage& operator=(const Hdf5Coverage&) = delete;
        ~Hdf5Coverage() override;

        [[nodiscard]] const CoverageDataset& dataset() const noexcept override { return dataset_; }
        // Null for an unknown feature, a time group, depth level or component it does not have,
        // or values that cannot be read.
        [[nodiscard]] Values readValues(std::string_view featureId, const CoverageSelection& selection) const override;

        // Features in the file this reader cannot draw, with the reason.
        [[nodiscard]] const std::vector<Skipped>& skipped() const noexcept { return skipped_; }

    private:
        Hdf5Coverage() = default;

        void readFeatures();

        std::int64_t file_ = -1;   // hid_t
        CoverageDataset dataset_;
        std::vector<Skipped> skipped_;

        mutable std::mutex mutex_;   // the HDF5 library and the cache
        mutable std::list<std::pair<std::string, Values>> cache_;   // most recently used first
    };
}
