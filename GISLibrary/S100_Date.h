#pragma once
#include <string>
#include <stdexcept>

namespace S100 {
    class S100_Date {
    public:
        std::tm tm = {};

        S100_Date() = default;
        S100_Date(const std::string& date);

        std::tm GetTm() const;
        std::string ToString() const;

    private:
        std::string NormalizeDate(const std::string& date);
    };
}
