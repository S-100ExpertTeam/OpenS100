#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

#include <list>
#include "StringUtil.h"

class Telephone :
	public XML_Item
{
public:
	Telephone();
	virtual ~Telephone();

private:
	std::list<std::string> voice;
	std::string facsimile = "";

public:
	void GetContents(pugi::xml_node& node);
	void SetVoice(std::string value);
	void SetVoice(std::wstring value) { SetVoice(toUtf8(value)); }
	void SetVoice(std::list<std::string> value);
	std::list<std::string>& GetVoicePointer();
	const std::string& GetVoice(int index);
	void SetFacsimile(std::string value);
	void SetFacsimile(std::wstring value) { SetFacsimile(toUtf8(value)); }
	const std::string& GetFacsimile();
	std::wstring GetFacsimileW() { return toWide(GetFacsimile()); }
};
