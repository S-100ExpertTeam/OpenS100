#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class Declaration
	{
	public:
		Declaration();
		virtual ~Declaration();

	public:
		std::wstring propertyName;
		std::wstring value;
	};
}