#pragma once

#include "IF_Attribute.h"

#include <string>
#include <vector>

namespace S100Interface
{
	class ComplexAttribute : public Attribute
	{
	public:
		virtual int GetSubAttributeCount() { return 0; }
		Attribute* GetSubAttribute(int index) { return nullptr; }
		bool IsSimple() override { return false; }
	};
}