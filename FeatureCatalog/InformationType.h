#pragma once
#include "S100ObjectType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class InformationType :
	public S100ObjectType
{
public:
	InformationType(); 
	virtual ~InformationType();

private:
	std::wstring superType = L"";
	std::list<std::wstring> subType; 

public:
	void GetContents(pugi::xml_node& node);
	const std::wstring& GetSuperType(); 
	void SetSuperType(std::wstring value);
	std::list<std::wstring>& GetSubTypePointer(); 
}; 