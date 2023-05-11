#include "stdafx.h"
#include "GF_ComplexAttributeType.h"

namespace GF
{
	ComplexAttributeType::ComplexAttributeType()
	{

	}
	
	ComplexAttributeType::~ComplexAttributeType()
	{
		for (auto i = carries.begin(); i != carries.end(); i++)
		{
			delete (*i);
		}

		carries.clear();
	}

	int ComplexAttributeType::GetSubAttributeCount()
	{
		return carries.size();
	}

	ThematicAttributeType* ComplexAttributeType::GetSubAttribute(int index)
	{
		if (index >= 0 && index < GetSubAttributeCount())
		{
			return carries.at(index);
		}

		return nullptr;
	}

	bool ComplexAttributeType::IsSimple()
	{
		return false;
	}
}