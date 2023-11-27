#pragma once


#include <stdexcept>
#include <string>

using namespace std;

namespace S100 {
	enum class S100_SupportFileRevisionStatus
	{
		_new = 1,
		replacement,
		deletion
	};

    inline string S100_SupportFileRevisionStatusToString(S100_SupportFileRevisionStatus category) {
        switch (category) {
        case S100_SupportFileRevisionStatus::_new: return "new";
        case S100_SupportFileRevisionStatus::replacement: return "replacement";
        case S100_SupportFileRevisionStatus::deletion: return "deletion";
        default: throw invalid_argument("Unknown category");
        }
    }

    inline S100_SupportFileRevisionStatus S100_SupportFileRevisionStatusFromString(const string& categoryName) {
        if (categoryName == "new") return S100_SupportFileRevisionStatus::_new;
        if (categoryName == "replacement") return S100_SupportFileRevisionStatus::replacement;
        if (categoryName == "deletion") return S100_SupportFileRevisionStatus::deletion;
        throw invalid_argument("Unknown category name");
    }

}