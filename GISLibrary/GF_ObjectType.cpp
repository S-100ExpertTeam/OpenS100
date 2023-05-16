#include "stdafx.h"
#include "GF_ObjectType.h"

namespace GF
{
	ObjectType::ObjectType()
	{

	}

	ObjectType::~ObjectType()
	{
		//for (auto i = attributes.begin(); i != attributes.end(); i++)
		//{
		//	delete (*i);
		//}
	}

	AttributeType* ObjectType::GetAttribute(int index)
	{
		//if (index >= 0 && GetAttributeCount())
		//{
		//	return attributes.at(index);
		//}

		return nullptr;
	}
}