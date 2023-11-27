#pragma once
#include <string.h>
#include <stdexcept>


namespace S100 {
	enum class S100_EncodingFormat
	{
		ISO_IEC_8211 = 1,
		GML = 2,
		HDF5 = 3,
		undefined = 100
	};


    inline std::string S100_EncodingFormatToString(S100_EncodingFormat category) {
        switch (category) {
        case S100_EncodingFormat::ISO_IEC_8211: return "ISO/IEC 8211";
        case S100_EncodingFormat::GML: return "GML";
        case S100_EncodingFormat::HDF5: return "HDF5";
        case S100_EncodingFormat::undefined: return "undefined";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    inline S100_EncodingFormat S100_EncodingFormatFromString(const std::string& categoryName) {
        if (categoryName == "ISO/IEC 8211") return S100_EncodingFormat::ISO_IEC_8211;
        if (categoryName == "GML") return S100_EncodingFormat::GML;
        if (categoryName == "HDF5") return S100_EncodingFormat::HDF5;
        if (categoryName == "undefined") return S100_EncodingFormat::undefined;
        throw std::invalid_argument("Unknown category name");
    }

}