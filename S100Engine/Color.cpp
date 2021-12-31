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

		auto token = node.text().get();
		this->token = pugi::as_wide(token);
	}

	void Color::SetTransparency(double value) 
	{
		transparency = value;
	}
	void Color::SetToken(std::wstring value) 
	{
		token = value;
	}

	double Color::GetTransparency() 
	{
		return transparency;
	}
	std::wstring Color::GetToken()
	{
		return token;
	}
}