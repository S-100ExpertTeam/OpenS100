#include "stdafx.h"
#include "NavigationPurpose.h"

namespace S100 {
    std::string S100_NavigationPurposeToString(NavigationPurpose category) {
        switch (category) {
        case NavigationPurpose::port: return "port";
        case NavigationPurpose::transit: return "transit";
        case NavigationPurpose::overview: return "overview";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    NavigationPurpose S100_NavigationPurposeFromString(const std::string& categoryName) {
        if (categoryName == "port") return NavigationPurpose::port;
        if (categoryName == "transit") return NavigationPurpose::transit;
        if (categoryName == "overview") return NavigationPurpose::overview;
        throw std::invalid_argument("Unknown category name");
    }
}
