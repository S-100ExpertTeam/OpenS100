#pragma once
#include <string>

class LanguagePackModelStatus {
public:
    enum Value {
        New,
        Modified,
        Deleted,
        Translated
    };

    LanguagePackModelStatus();
    LanguagePackModelStatus(Value v);
    LanguagePackModelStatus(const std::string& str);

    operator Value() const;

    std::string toString() const;
    void fromString(const std::string& str);

private:
    Value value;
};

