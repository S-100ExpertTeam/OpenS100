#pragma once
#include "S100_Line.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>
namespace S100_SVG_D2D1_DLL 
{
	class S100_LineStyle
	{
	public:
		S100_LineStyle();
		virtual ~S100_LineStyle();

	public:
		std::wstring m_filepath;
		S100_Line line;
		S100_Line ReadLineStyleByPugi(std::wstring path);

	private:
		void GetLineStyleInfo(pugi::xml_node node);
		void GetPen(pugi::xml_node node);
		void GetDash(pugi::xml_node node);
		void GetSymbol(pugi::xml_node node);
	};
}