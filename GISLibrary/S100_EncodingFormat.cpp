#include "stdafx.h"
#include "S100_EncodingFormat.h"

namespace S100 {
    std::string S100_EncodingFormatToString(S100_EncodingFormat category) {
        switch (category) {
        case S100_EncodingFormat::ISO_IEC_8211: return "ISO/IEC 8211";
        case S100_EncodingFormat::GML: return "GML";
        case S100_EncodingFormat::HDF5: return "HDF5";
        case S100_EncodingFormat::undefined: return "undefined";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_EncodingFormat S100_EncodingFormatFromString(const std::string& categoryName) {
        if (categoryName == "ISO/IEC 8211") return S100_EncodingFormat::ISO_IEC_8211;
        if (categoryName == "GML") return S100_EncodingFormat::GML;
        if (categoryName == "HDF5") return S100_EncodingFormat::HDF5;
        if (categoryName == "undefined") return S100_EncodingFormat::undefined;
        throw std::invalid_argument("Unknown category name");
    }
}
