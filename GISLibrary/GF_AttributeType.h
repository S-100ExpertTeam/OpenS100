#pragma once

#include <string>

namespace GF
{
	class AttributeType
	{
	public:
		virtual std::string GetCode() { return ""; }
		virtual bool IsSimple() { return false; }
	};
}