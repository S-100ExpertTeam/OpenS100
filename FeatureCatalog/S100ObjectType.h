#pragma once
#include "NamedType.h"
#include "InformationBinding.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class S100ObjectType :
	public NamedType
{
public:
	S100ObjectType();
	virtual ~S100ObjectType();

private:
	std::unordered_map<std::wstring, InformationBinding> informationBinding;

public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<std::wstring, InformationBinding>& GetInformationBindingPointer();
};