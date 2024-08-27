#include "stdafx.h"
#include "GF_ComplexAttributeType.h"
#include "GF_SimpleAttributeType.h"

namespace GF
{
	ComplexAttributeType::ComplexAttributeType()
	{

	}

	ComplexAttributeType::ComplexAttributeType(const ComplexAttributeType& other)
		: ThematicAttributeType(other)
	{
		for (const auto& iter : other.carries)
		{
			if (iter->IsSimple())
			{
				SimpleAttributeType* sa = dynamic_cast<SimpleAttributeType*>(iter);
				if (sa)
				{
					SimpleAttributeType* attribute = new SimpleAttributeType(*sa);
					carries.push_back(attribute);
				}
				else
				{
					ThematicAttributeType* attribute = new ThematicAttributeType(*iter);
					carries.push_back(attribute);
				}
			}
			else
			{
				ComplexAttributeType* ca = dynamic_cast<ComplexAttributeType*>(iter);
				if (ca)
				{
					ComplexAttributeType* attribute = new ComplexAttributeType(*ca);
					carries.push_back(attribute);
				}
				else
				{
					ThematicAttributeType* attribute = new ThematicAttributeType(*iter);
					carries.push_back(attribute);
				}
			}
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

	int ComplexAttributeType::GetSubAttributeCount(std::string code) const
	{
		int count = 0;
		for (auto iter : carries)
		{
			if (iter->GetCode() == code)
			{
				count++;
			}
		}

		return count;
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

	ThematicAttributeType* ComplexAttributeType::AddSubAttribute(ThematicAttributeType* subAttribute)
	{
		carries.push_back(subAttribute);
		return subAttribute;
	}

	SimpleAttribute* ComplexAttributeType::AddSubSimpleAttribute(
		FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value)
	{
		auto sa = new SimpleAttributeType(valueType, value);
		sa->SetCode(code);
		return (SimpleAttribute*)AddSubAttribute(sa);
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

	std::optional<ThematicAttributeType*> ComplexAttributeType::getThematicAttribute(std::string code, int index)
	{
		int num = 0;
		for (auto iter : carries)
		{
			if (iter->GetCode() == code)
			{
				if (num == index)
				{
					return iter;
				}
				num++;
			}
		}

		return std::nullopt;
	}

	std::optional<ComplexAttributeType*> ComplexAttributeType::getComplexAttribute(std::string code, int index)
	{
		int num = 0;
		for (auto iter : carries)
		{
			if (iter->IsSimple() == false && iter->GetCode() == code)
			{
				if (num == index)
				{
					return (ComplexAttributeType*)iter;
				}
				num++;
			}
		}

		return std::nullopt;
	}

	std::vector<std::string> ComplexAttributeType::getAttributeValues(std::string code)
	{
		std::vector<std::string> values;
		for (auto iter : carries)
		{
			if (iter->GetCode() == code && iter->IsSimple())
			{
				values.push_back(iter->GetValue());
			}
		}

		return values;
	}
}