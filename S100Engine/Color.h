#pragma once

#include <pugixml.hpp>

#include <string>

namespace GraphicBasePackage
{
	class Color
	{
	public:
		Color();
		virtual ~Color();

	private:
		std::string token;
		double transparency = 0;

	public:
		void GetContents(pugi::xml_node& node);
		void SetTransparency(double value);
		void SetToken(const std::string& value);
		void SetToken(const std::wstring& value);

		double GetTransparency();
		std::string  GetToken();
		std::wstring GetTokenW();
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
		std::string token;
		std::string name;
		std::string desctiption;
	};
}