#include "stdafx.h"
#include "Color.h"

namespace libS100Engine
{
	Color::Color()
	{
	}

	Color::~Color()
	{

	}
}

namespace GraphicBasePackage
{
	Color::Color()
	{

	}

	Color::~Color()
	{

	}

	void Color::GetContents(pugi::xml_node& node)
	{
		auto attribute = node.attribute("transparency");
		if (attribute)
		{
			transparency = attribute.as_double();
		}

		token = node.text().get();
	}

	void Color::SetTransparency(double value)
	{
		transparency = value;
	}

	void Color::SetToken(const std::string& value)
	{
		token = value;
	}

	void Color::SetToken(const std::wstring& value)
	{
		token = LibMFCUtil::WStringToString(value);
	}

	double Color::GetTransparency()
	{
		return transparency;
	}

	std::string Color::GetToken()
	{
		return token;
	}

	std::wstring Color::GetTokenW()
	{
		return LibMFCUtil::StringToWString(token);
	}
}