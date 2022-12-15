#pragma once

#include "Attribute.h"
#include "UnitOfMeasure.h"
#include "AttributeConstraints.h"
#include "S100_CD_AttributeValueType.h"
#include "S100_CD_QuantitySpecification.h"
#include "ListedValue.h"

#include <pugixml.hpp>

#include <vector>

// S100_FC_SimpleAttribute
class SimpleAttribute : public Attribute
{
public:
	SimpleAttribute();
	virtual ~SimpleAttribute();

private:
	FCD::S100_CD_AttributeValueType valueType = FCD::S100_CD_AttributeValueType::none;
	UnitOfMeasure uom; 
	FCD::S100_CD_QuantitySpecification quantitySpecification;
	AttributeConstraints constraints; 
	
	std::vector<ListedValue*> listedValue;

	// Key : Code
	// Value : Label
	std::unordered_map<int, ListedValue*> codeMap;
	
	std::unordered_map<std::wstring, ListedValue*> labelMap;

public:
	void GetContents(pugi::xml_node& node);

	UnitOfMeasure& GetUOMRef();

	FCD::S100_CD_AttributeValueType GetValueType();
	void SetValueType(FCD::S100_CD_AttributeValueType value);
	
	FCD::S100_CD_QuantitySpecification GetQuantitySpecification();
	void SetQuantitySpecification(FCD::S100_CD_QuantitySpecification value);
	
	AttributeConstraints& GetConstraintsPointer(); 
	std::vector<ListedValue*>& GetListedValuePointer();

	void InsertListedValue(ListedValue* item);
	ListedValue* GetListedValue(std::wstring label);
	ListedValue* GetListedValue(int code);

	bool IsSimple();
	bool IsComplex();
};