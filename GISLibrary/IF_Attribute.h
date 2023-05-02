#pragma once

#include <string>

namespace S100Interface
{
	class Attribute
	{
	public:
		virtual std::string GetCode() { return ""; }
		virtual bool IsSimple() { return false; }
	};
}