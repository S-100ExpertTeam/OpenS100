#pragma once
#include <string>

#include "..\\extlibs\pugixml\include\pugixml.hpp"
 
class S100_LineSymbol
{
public:
	S100_LineSymbol();
	virtual ~S100_LineSymbol();

public:
	// attribute
	std::wstring reference;
	std::wstring rotation;
	std::wstring scaleFactor;
	std::wstring crsType;

	// element
	std::wstring position;

public:
	void GetContents(pugi::xml_node& node);

	void SetReference(std::wstring& value);
	void SetRotation(std::wstring& value);
	void SetScaleFactor(std::wstring& value);
	void SetCrsType(std::wstring& value);
	void SetPosition(std::wstring& value);

	std::wstring GetReference();
	std::wstring GetRotation();
	std::wstring GetScaleFactor();
	std::wstring GetCrsType();
	std::wstring GetPosition();
};