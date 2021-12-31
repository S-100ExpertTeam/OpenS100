#pragma once
#include "Item.h"
#include "AttributeBinding.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ComplexAttribute :
	public Item
{
public:
	ComplexAttribute();
	virtual ~ComplexAttribute();

private:
	std::list<AttributeBinding> subAttributeBinding;

public:
	void GetContents(pugi::xml_node& node);

	void SetSubAttributeBinding(AttributeBinding value);
	void SetSubAttributeBinding(std::list<AttributeBinding> value);

	AttributeBinding GetSubAttributeBinding(int index);
	std::list<AttributeBinding>& GetSubAttributeBindingPointer();
};