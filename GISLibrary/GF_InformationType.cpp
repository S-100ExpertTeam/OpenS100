#include "stdafx.h"
#include "GF_InformationType.h"

namespace GF
{
	InformationType::InformationType()
	{

	}

	InformationType::InformationType(const InformationType& other) : ObjectType(other)
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

	InformationType InformationType::operator=(const InformationType& other)
	{
		ObjectType::operator=(other);

		return *this;
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