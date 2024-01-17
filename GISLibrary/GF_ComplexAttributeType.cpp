#include "stdafx.h"
#include "GF_ComplexAttributeType.h"

namespace GF
{
	ComplexAttributeType::ComplexAttributeType()
	{

	}

	ComplexAttributeType::ComplexAttributeType(const ComplexAttributeType& other)
		: ThematicAttributeType(other)
	{
		int cnt = other.GetSubAttributeCount();
		for (int i = 0; i < cnt; i++) {
			auto item = new ThematicAttributeType(*other.GetSubAttribute(i));
			AddSubAttribute(item);
		}
	}
	
	ComplexAttributeType::~ComplexAttributeType()
	{
		for (auto i = carries.begin(); i != carries.end(); i++)
		{
			delete (*i);
		}

		carries.clear();
	}

	int ComplexAttributeType::GetSubAttributeCount() const
	{
		return (int)carries.size();
	}

	ThematicAttributeType* ComplexAttributeType::GetSubAttribute(int index) const
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

	void ComplexAttributeType::AddSubAttribute(ThematicAttributeType* subAttribute)
	{
		carries.push_back(subAttribute);
	}

	void ComplexAttributeType::AddSubSimpleAttribute(
		FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value)
	{
		auto sa = new SimpleAttributeType(valueType, value);
		sa->SetCode(code);
		AddSubAttribute(sa);
	}

	ComplexAttributeType* ComplexAttributeType::AddComplexAttribute(std::string code)
	{
		auto ca = new ComplexAttributeType();
		ca->SetCode(code);
		carries.push_back(ca);
		return ca;
	}

	ThematicAttributeType* ComplexAttributeType::clone()
	{
		return new ComplexAttributeType(*this);
	}
}