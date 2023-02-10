#pragma once
#include "NamedType.h"
#include "InformationBinding.h"

#include <pugixml.hpp>

#include <list>

class S100ObjectType :
	public NamedType
{
public:
	S100ObjectType();
	virtual ~S100ObjectType();

private:
	std::list<InformationBinding*> informationBinding;

public:
	void GetContents(pugi::xml_node& node);
	std::list<InformationBinding*>& GetInformationBindingPointer();

	void InsertInformationBinding(InformationBinding* value);

	InformationBinding* CanMerged(InformationBinding& ib);
};