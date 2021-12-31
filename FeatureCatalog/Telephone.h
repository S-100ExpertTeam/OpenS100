#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <list>

class Telephone :
	public XML_Item
{
public:
	Telephone();
	virtual ~Telephone();

private:
	std::list<std::wstring> voice;
	std::wstring facsimile = L"";

public:
	void GetContents(pugi::xml_node& node);
	void SetVoice(std::wstring value);
	void SetVoice(std::list<std::wstring> value);
	std::list<std::wstring>& GetVoicePointer();
	const std::wstring& GetVoice(int index);
	void SetFacsimile(std::wstring value);
	const std::wstring& GetFacsimile();
};