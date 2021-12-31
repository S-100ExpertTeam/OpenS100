#pragma once

#include <string>
#include <unordered_map>

namespace S100_SVG_D2D1_DLL
{
	class UnitOfCSS;
	class CSSStyle;
	class CSS
	{
	public:
		CSS();
		virtual ~CSS();

	public:
		std::unordered_map<std::wstring, UnitOfCSS*> m_unitMap; //Collection of colors.

	public:
		bool ReadCSSFile(std::wstring path);
		bool OrganizeCSS();
		CSSStyle* CSS::GetCSSStyle(std::wstring _propertyName, std::wstring _value);
	};
}
