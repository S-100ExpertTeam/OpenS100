#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class IHO_Description
	{
	public:
		IHO_Description();
		virtual ~IHO_Description();

	public:
		std::wstring publisher;
		std::wstring creationDate;
		std::wstring source;
		std::wstring format;
		std::wstring version;
	};
}