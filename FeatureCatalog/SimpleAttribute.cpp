#include "stdafx.h"
#include "SimpleAttribute.h"
#include "S100_CD_AttributeValueType.h"

#include "../LibMFCUtil/LibMFCUtil.h" 

SimpleAttribute::SimpleAttribute()
{

}

SimpleAttribute::~SimpleAttribute()
{
	for (auto i = listedValue.begin(); i != listedValue.end(); i++)
	{
		delete (*i);
	}
}

void SimpleAttribute::GetContents(pugi::xml_node& node)
{
	((Attribute*)this)->Attribute::GetContents(node);

	for (auto i = node.first_child(); i; i = i.next_sibling())
	{
		const pugi::char_t* instructionName = i.name();

		if (!strcmp(instructionName, "S100FC:valueType"))
		{
			std::string value = i.child_value();
			valueType = FCD::StringToS100_CD_AttributeValueType(value);
		}
		else if (!strcmp(instructionName, "S100FC:uom"))
		{
			uom.GetContents(i);
		}
		else if (!strcmp(instructionName, "S100FC:quantitySpecification"))
		{
			std::string value = i.child_value();
			quantitySpecification = FCD::StringToS100_CD_QuantitySpecification(value);
		}
		else if (!strcmp(instructionName, "S100FC:constraints"))
		{
			constraints.GetContents(i);
		} 
		else if (!strcmp(instructionName, "S100FC:listedValues"))
		{
			for (auto j = i.first_child(); j; j = j.next_sibling())
			{
				const pugi::char_t* instructionName = j.name();

				if (!strcmp(instructionName, "S100FC:listedValue"))
				{
					auto lv = new ListedValue();
					lv->GetContents(j);
					InsertListedValue(lv);
				}
			}
		}
	}
}

UnitOfMeasure& SimpleAttribute::GetUOMRef()
{
	return uom;
}

FCD::S100_CD_AttributeValueType SimpleAttribute::GetValueType() 
{
	return valueType;
}

void SimpleAttribute::SetValueType(FCD::S100_CD_AttributeValueType value)
{
	valueType = value;
}

FCD::S100_CD_QuantitySpecification SimpleAttribute::GetQuantitySpecification()
{
	return quantitySpecification;
}

void SimpleAttribute::SetQuantitySpecification(FCD::S100_CD_QuantitySpecification value)
{
	quantitySpecification = value;
}

AttributeConstraints& SimpleAttribute::GetConstraintsPointer()
{
	return constraints;
}

std::vector<ListedValue*>& SimpleAttribute::GetListedValuePointer()
{
	return listedValue;
}

void SimpleAttribute::InsertListedValue(ListedValue* item)
{
	listedValue.push_back(item);
	codeMap.insert({ item->GetCode(), item });
	labelMap.insert({ item->GetLabel(), item });
}

ListedValue* SimpleAttribute::GetListedValue(std::wstring label)
{
	auto item = labelMap.find(label);

	if (item != labelMap.end())
	{
		return item->second;
	}

	return nullptr;
}

ListedValue* SimpleAttribute::GetListedValue(int code)
{
	auto item = codeMap.find(code);

	if (item != codeMap.end())
	{
		return item->second;
	}

	return nullptr;
}