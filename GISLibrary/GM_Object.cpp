#include "stdafx.h"
#include "GM_Object.h"

namespace GM
{
	Object::Object()
	{

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
}