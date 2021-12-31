#pragma once

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

namespace GraphicBasePackage
{
	class Color
	{
	public:
		Color();
		virtual ~Color();

	private:
		std::wstring token;
		double transparency = 0;

	public:
		void GetContents(pugi::xml_node& node);
		void SetTransparency(double value);
		void SetToken(std::wstring value);

		double GetTransparency();
		std::wstring GetToken();
	};
}

namespace libS100Engine
{
	class Color
	{
	public:
		Color();
		virtual ~Color();

	public:
		std::wstring token;
		std::wstring name;
		std::wstring desctiption;
	};
}