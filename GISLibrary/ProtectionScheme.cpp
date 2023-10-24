#include "stdafx.h"
#include "ProtectionScheme.h"

namespace S100
{
	ProtectionScheme stringToProtectionScheme(std::string value)
	{
		if (value == "S100p15") return ProtectionScheme::S100p15;
		else return ProtectionScheme::none;
	}

	std::string protectionSchemeToString(ProtectionScheme value)
	{
		switch (value)
		{
		case ProtectionScheme::none: return "";
		case ProtectionScheme::S100p15: return "S100p15";
		default: return "";
		}
	}
}