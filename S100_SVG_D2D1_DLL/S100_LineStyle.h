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
		S100_Line ReadLineStyle(std::wstring path);
		S100_Line ReadLineStyleByPugi(std::wstring path);

	private:
		void GetLineStyleInfo(MSXML2::IXMLDOMNodePtr pNode);
		void GetLineStyleInfo(pugi::xml_node node);

		void GetPen(MSXML2::IXMLDOMNodePtr pNode);
		void GetPen(pugi::xml_node node);

		void GetDash(MSXML2::IXMLDOMNodeListPtr pNodeList);
		void GetDash(pugi::xml_node node);

		void GetSymbol(MSXML2::IXMLDOMNodePtr pNode);
		void GetSymbol(pugi::xml_node node);
	};
}