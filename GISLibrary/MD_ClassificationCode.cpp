#include "stdafx.h"
#include "MD_ClassificationCode.h"

namespace S100 {
    std::string MD_ClassificationCodeToString(MD_ClassificationCode category) {
        switch (category) {
        case MD_ClassificationCode::confidential: return "confidential";
        case MD_ClassificationCode::forOfficialUseOnly: return "forOfficialUseOnly";
        case MD_ClassificationCode::limitedDistribution: return "limitedDistribution";
        case MD_ClassificationCode::_protected: return "protected";
        case MD_ClassificationCode::restricted: return "restricted";
        case MD_ClassificationCode::SBU: return "SBU";
        case MD_ClassificationCode::secret: return "secret";
        case MD_ClassificationCode::topSecret: return "topSecret";
        case MD_ClassificationCode::unclassified: return "unclassified";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    MD_ClassificationCode MD_ClassificationCodeFromString(const std::string& categoryName) {
        if (categoryName == "confidential") return MD_ClassificationCode::confidential;
        if (categoryName == "forOfficialUseOnly") return MD_ClassificationCode::forOfficialUseOnly;
        if (categoryName == "limitedDistribution") return MD_ClassificationCode::limitedDistribution;
        if (categoryName == "protected") return MD_ClassificationCode::_protected;
        if (categoryName == "restricted") return MD_ClassificationCode::restricted;
        if (categoryName == "SBU") return MD_ClassificationCode::SBU;
        if (categoryName == "secret") return MD_ClassificationCode::secret;
        if (categoryName == "topSecret") return MD_ClassificationCode::topSecret;
        if (categoryName == "unclassified") return MD_ClassificationCode::unclassified;
        throw std::invalid_argument("Unknown category name");
    }
}
