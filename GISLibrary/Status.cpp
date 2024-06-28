#include "stdafx.h"
#include "Status.h"

LanguagePackModelStatus::LanguagePackModelStatus() : value(New) {}

LanguagePackModelStatus::LanguagePackModelStatus(Value v) : value(v) {}

LanguagePackModelStatus::LanguagePackModelStatus(const std::string& str) {
    fromString(str);
}

LanguagePackModelStatus::operator Value() const {
    return value;
}

std::string LanguagePackModelStatus::toString() const {
    switch (value) {
    case New: return "New";
    case Modified: return "Modified";
    case Deleted: return "Deleted";
    case Translated: return "Translated";
    default: return "Unknown";
    }
}

void LanguagePackModelStatus::fromString(const std::string& str) {
    if (str == "New") value = New;
    else if (str == "Modified") value = Modified;
    else if (str == "Deleted") value = Deleted;
    else if (str == "Translated") value = Translated;
    else value = New; // Default case
}
