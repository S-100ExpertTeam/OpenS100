#include "stdafx.h"
#include "GF_NamedType.h"

namespace GF
{
	std::string NamedType::GetCode()
	{
		return code;
	}

	void NamedType::SetCode(std::string& value)
	{
		code = value;
	}
}