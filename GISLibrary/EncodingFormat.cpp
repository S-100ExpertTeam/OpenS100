#include "stdafx.h"
#include "EncodingFormat.h"

namespace S100 {
    std::string S100_EncodingFormatToString(EncodingFormat category) {
        switch (category) {
        case EncodingFormat::ISO_IEC_8211: return "ISO/IEC 8211";
        case EncodingFormat::GML: return "GML";
        case EncodingFormat::HDF5: return "HDF5";
        case EncodingFormat::undefined: return "undefined";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    EncodingFormat S100_EncodingFormatFromString(const std::string& categoryName) {
        if (categoryName == "ISO/IEC 8211") return EncodingFormat::ISO_IEC_8211;
        if (categoryName == "GML") return EncodingFormat::GML;
        if (categoryName == "HDF5") return EncodingFormat::HDF5;
        if (categoryName == "undefined") return EncodingFormat::undefined;
        throw std::invalid_argument("Unknown category name");
    }
}
