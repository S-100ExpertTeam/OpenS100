#pragma once

#include <string>

namespace S100
{

	enum class ProtectionScheme
	{
		none = 0,
		S100p15 = 1
	};

	ProtectionScheme stringToProtectionScheme(std::string value);
	std::string protectionSchemeToString(ProtectionScheme value);
}