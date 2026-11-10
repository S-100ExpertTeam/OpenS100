#include "Records.h"

namespace s100::sdb
{
    std::string_view toString(DatasetFormat format) noexcept
    {
        switch (format)
        {
        case DatasetFormat::Iso8211: return "iso8211";
        case DatasetFormat::Gml:     return "gml";
        case DatasetFormat::Hdf5:    return "hdf5";
        }
        return "iso8211";
    }

    std::optional<DatasetFormat> parseDatasetFormat(std::string_view text) noexcept
    {
        if (text == "iso8211") return DatasetFormat::Iso8211;
        if (text == "gml")     return DatasetFormat::Gml;
        if (text == "hdf5")    return DatasetFormat::Hdf5;
        return std::nullopt;
    }

    std::string_view toString(DatasetStatus status) noexcept
    {
        switch (status)
        {
        case DatasetStatus::Active:    return "active";
        case DatasetStatus::Cancelled: return "cancelled";
        }
        return "active";
    }

    std::optional<DatasetStatus> parseDatasetStatus(std::string_view text) noexcept
    {
        if (text == "active")    return DatasetStatus::Active;
        if (text == "cancelled") return DatasetStatus::Cancelled;
        return std::nullopt;
    }
}
