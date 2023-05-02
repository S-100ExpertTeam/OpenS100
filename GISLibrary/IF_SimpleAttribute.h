#pragma once

#include "IF_Attribute.h"

#include <string>

namespace S100Interface 
{
	class SimpleAttribute : public Attribute
	{
	public:
		virtual std::string GetValue() { return ""; }
		bool IsSimple() override { return true; }
	};
}