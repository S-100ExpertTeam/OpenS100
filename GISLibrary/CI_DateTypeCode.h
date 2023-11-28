#pragma once

#include <string>
#include <stdexcept>

namespace S100
{
    enum class CI_DateTypeCode
    {
        adopted = 1,
        creation,
        deprecated,
        distribution,
        expiry,
        inForce,
        lastRevision,
        lastUpdate,
        nextUpdate,
        publication,
        released,
        revision,
        superseded,
        unavailable,
        validityBegins,
        validityExpires
    };

    std::string CI_DateTypeCodeToString(CI_DateTypeCode category);
    CI_DateTypeCode CI_DateTypeCodeFromString(const std::string& categoryName);
}