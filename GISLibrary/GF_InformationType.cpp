#include "stdafx.h"
#include "GF_InformationType.h"

namespace GF
{
	InformationType::InformationType()
	{

	}

	InformationType::~InformationType()
	{
		//for (auto i = attributes.begin(); i != attributes.end(); i++)
		//{
		//	delete (*i);
		//}

		//attributes.clear();
	}

	int InformationType::GetAttributeCount()
	{
		return ObjectType::GetAttributeCount();
	}

	//ThematicAttributeType* InformationType::GetAttribute(int index)
	//{
	//	if (index >= 0 && index < GetAttributeCount())
	//	{
	//		return attributes.at(index);
	//	}

	//	return nullptr;
	//}

	//std::string InformationType::GetAttributeValue(int index)
	//{
	//	auto attr = GetAttribute(index);
	//	if (attr)
	//	{
	//		if (attr->IsSimple())
	//		{
	//			return attr->GetValue();
	//		}
	//	}

	//	return "";
	//}

	//int InformationType::GetParentAttributeIndex(int index)
	//{
	//	return 0;
	//}
}