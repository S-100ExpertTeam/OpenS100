#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class EncodingFormat {
        ISO_IEC_8211 = 1,
        GML = 2,
        HDF5 = 3,
        undefined = 100
    };

    std::string S100_EncodingFormatToString(EncodingFormat category);
    EncodingFormat S100_EncodingFormatFromString(const std::string& categoryName);
}
