#pragma once
#include "Item.h"
#include "AttributeBinding.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class NamedType :
	public Item
{
public:
	NamedType();
	virtual ~NamedType();

private:
	std::list<AttributeBinding> attributeBinding;

public:
	void GetContents(pugi::xml_node& node);
	std::list<AttributeBinding>& GetAttributeBindingPointer();
};