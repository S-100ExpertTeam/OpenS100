#pragma once
#include "S100ObjectType.h"

#include <pugixml.hpp>
#include "StringUtil.h"

class InformationType :
	public S100ObjectType
{
public:
	InformationType(); 
	virtual ~InformationType();

private:
	std::string superType = "";
	std::list<std::string> subType; 

public:
	void GetContents(pugi::xml_node& node);
	const std::string& GetSuperType(); 
	void SetSuperType(std::string value);
	void SetSuperType(const char* value);
	void SetSuperType(std::wstring value);
	std::list<std::string>& GetSubTypePointer(); 
}; 
