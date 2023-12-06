#include "stdafx.h"
#include "CI_OnLineFunctionCode.h"

namespace S100
{
    std::string CI_OnLineFunctionCodeToString(CI_OnLineFunctionCode category)
    {
        switch (category)
        {
        case CI_OnLineFunctionCode::browseGraphic: return "browseGraphic";
        case CI_OnLineFunctionCode::browsing: return "browsing";
        case CI_OnLineFunctionCode::completeMetadata: return "completeMetadata";
        case CI_OnLineFunctionCode::download: return "download";
        case CI_OnLineFunctionCode::emailService: return "emailService";
        case CI_OnLineFunctionCode::fileAccess: return "fileAccess";
        case CI_OnLineFunctionCode::information: return "information";
        case CI_OnLineFunctionCode::offlineAccess: return "offlineAccess";
        case CI_OnLineFunctionCode::order: return "order";
        case CI_OnLineFunctionCode::search: return "search";
        case CI_OnLineFunctionCode::upload: return "upload";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    CI_OnLineFunctionCode CI_OnLineFunctionCodeFromString(const std::string& categoryName)
    {
        if (categoryName == "browseGraphic") return CI_OnLineFunctionCode::browseGraphic;
        if (categoryName == "browsing") return CI_OnLineFunctionCode::browsing;
        if (categoryName == "completeMetadata") return CI_OnLineFunctionCode::completeMetadata;
        if (categoryName == "download") return CI_OnLineFunctionCode::download;
        if (categoryName == "emailService") return CI_OnLineFunctionCode::emailService;
        if (categoryName == "fileAccess") return CI_OnLineFunctionCode::fileAccess;
        if (categoryName == "information") return CI_OnLineFunctionCode::information;
        if (categoryName == "offlineAccess") return CI_OnLineFunctionCode::offlineAccess;
        if (categoryName == "order") return CI_OnLineFunctionCode::order;
        if (categoryName == "search") return CI_OnLineFunctionCode::search;
        if (categoryName == "upload") return CI_OnLineFunctionCode::upload;
        throw std::invalid_argument("Unknown category name");
    }
}
