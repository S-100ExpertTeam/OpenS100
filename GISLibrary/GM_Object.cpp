#include "stdafx.h"
#include "GM_Object.h"

#include <regex>

namespace GM
{
	Object::Object()
	{

	}

	Object::Object(std::string id)
	{
		this->id = id;
	}

	Object::Object(const Object& item)
	{
		this->id = item.id;
	}

	Object::~Object()
	{

	}

	std::string Object::GetID()
	{
		return id;
	}

	std::wstring Object::GetIDAsWString()
	{
		return pugi::as_wide(id);
	}

	int Object::GetIDAsInt()
	{
		std::string digitID = std::regex_replace(id, std::regex(R"([\D])"), "");
		return std::stoi(digitID);
	}

	void Object::SetID(std::string value)
	{
		id = value;
	}

	void Object::SetID(std::wstring value)
	{
		id = pugi::as_utf8(value);
	}

	void Object::SetID(int value)
	{
		id = std::to_string(value);
	}

	std::string Object::getSRSName()
	{
		return srsName;
	}

	void Object::setSRSName(std::string value)
	{
		this->srsName = value;
	}

	void Object::setParentIdSrsName(std::string id, std::string srsName)
	{
		SetID(id);
		setSRSName(srsName);
	}

	void Object::readID(pugi::xml_node& node)
	{
		this->id = node.attribute("gml:id").value();
	}

	void Object::readSRSName(pugi::xml_node& node)
	{
		this->srsName = node.attribute("srsName").value();
	}

	void Object::readIdSRSName(pugi::xml_node& node)
	{
		if (this->id.empty()) {
			readID(node);
		}
		
		if (this->srsName.empty()) {
			readSRSName(node);
		}
	}
}