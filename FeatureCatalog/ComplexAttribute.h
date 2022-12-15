#pragma once

#include "Attribute.h"
#include "AttributeBinding.h"

#include <pugixml.hpp>

class ComplexAttribute :
	public Attribute
{
public:
	ComplexAttribute();
	virtual ~ComplexAttribute();

private:
	std::list<AttributeBinding*> subAttributeBinding;

public:
	void GetContents(pugi::xml_node& node);

	void SetSubAttributeBinding(AttributeBinding* value);
	void SetSubAttributeBinding(std::list<AttributeBinding*> value);

	AttributeBinding* GetSubAttributeBinding(int index);
	std::list<AttributeBinding*>& GetSubAttributeBindingPointer();
	std::list<AttributeBinding*>& GetAttributeBindingList();

	bool IsSimple();
	bool IsComplex();
};