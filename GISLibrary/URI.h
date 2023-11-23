#pragma once
#include <string>

class URI : public std::string {
public:
	using std::string::string;
	using std::string::operator=;
};