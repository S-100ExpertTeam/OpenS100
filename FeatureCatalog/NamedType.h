#pragma once
#include "Item.h"
#include "AttributeBinding.h"

#include <pugixml.hpp>

class NamedType :
	public Item
{
public:
	NamedType();
	virtual ~NamedType();

private:
	bool isAbstract = false;
	std::list<AttributeBinding*> attributeBinding;

public:
	void GetContents(pugi::xml_node& node);
	std::list<AttributeBinding*>& GetAttributeBindingPointer();

	void SetIsAbstract(bool value);
	bool GetIsAbstract();

	void InsertAttributeBinding(AttributeBinding* ab);
};