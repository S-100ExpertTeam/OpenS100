#pragma once

#include <string>
#include <stdexcept>

namespace S100
{
    enum class CI_OnLineFunctionCode
    {
        browseGraphic,
        browsing,
        completeMetadata,
        download,
        emailService,
        fileAccess,
        information,
        offlineAccess,
        order,
        search,
        upload
    };

    std::string CI_OnLineFunctionCodeToString(CI_OnLineFunctionCode category);
    CI_OnLineFunctionCode CI_OnLineFunctionCodeFromString(const std::string& categoryName);
}
