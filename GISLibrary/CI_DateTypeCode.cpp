#include "stdafx.h"
#include "CI_DateTypeCode.h"

namespace S100
{
    std::string CI_DateTypeCodeToString(CI_DateTypeCode category)
    {
        switch (category)
        {
        case CI_DateTypeCode::adopted: return "adopted";
        case CI_DateTypeCode::creation: return "creation";
        case CI_DateTypeCode::deprecated: return "deprecated";
        case CI_DateTypeCode::distribution: return "distribution";
        case CI_DateTypeCode::expiry: return "expiry";
        case CI_DateTypeCode::inForce: return "inForce";
        case CI_DateTypeCode::lastRevision: return "lastRevision";
        case CI_DateTypeCode::lastUpdate: return "lastUpdate";
        case CI_DateTypeCode::nextUpdate: return "nextUpdate";
        case CI_DateTypeCode::publication: return "publication";
        case CI_DateTypeCode::released: return "released";
        case CI_DateTypeCode::revision: return "revision";
        case CI_DateTypeCode::superseded: return "superseded";
        case CI_DateTypeCode::unavailable: return "unavailable";
        case CI_DateTypeCode::validityBegins: return "validityBegins";
        case CI_DateTypeCode::validityExpires: return "validityExpires";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    CI_DateTypeCode CI_DateTypeCodeFromString(const std::string& categoryName)
    {
        if (categoryName == "adopted") return CI_DateTypeCode::adopted;
        if (categoryName == "creation") return CI_DateTypeCode::creation;
        if (categoryName == "deprecated") return CI_DateTypeCode::deprecated;
        if (categoryName == "distribution") return CI_DateTypeCode::distribution;
        if (categoryName == "expiry") return CI_DateTypeCode::expiry;
        if (categoryName == "inForce") return CI_DateTypeCode::inForce;
        if (categoryName == "lastRevision") return CI_DateTypeCode::lastRevision;
        if (categoryName == "lastUpdate") return CI_DateTypeCode::lastUpdate;
        if (categoryName == "nextUpdate") return CI_DateTypeCode::nextUpdate;
        if (categoryName == "publication") return CI_DateTypeCode::publication;
        if (categoryName == "released") return CI_DateTypeCode::released;
        if (categoryName == "revision") return CI_DateTypeCode::revision;
        if (categoryName == "superseded") return CI_DateTypeCode::superseded;
        if (categoryName == "unavailable") return CI_DateTypeCode::unavailable;
        if (categoryName == "validityBegins") return CI_DateTypeCode::validityBegins;
        if (categoryName == "validityExpires") return CI_DateTypeCode::validityExpires;
        throw std::invalid_argument("Unknown category name");
    }
}