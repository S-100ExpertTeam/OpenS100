#include "stdafx.h"
#include "IF_ObjectType.h"

namespace S100Interface
{
	ObjectType::ObjectType()
	{

	}

	ObjectType::~ObjectType()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			delete (*i);
		}
	}

	Attribute* ObjectType::GetAttribute(int index)
	{
		if (index >= 0 && GetAttributeCount())
		{
			return attributes.at(index);
		}

		return nullptr;
	}
}