#pragma once

#include <string>

#include "LineStylesPackage.h"
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace S100XMLReader
{
	class S100PCLineStylesReader
	{
	public:
		S100PCLineStylesReader();
		virtual ~S100PCLineStylesReader();

	public:
		static bool OpenByPugi(std::wstring path, LineStylesPackage::LineStyles *pLineStyles);

	private:
		static bool SetLineStyle(pugi::xml_node node, LineStylesPackage::AbstractLineStyle **pLineStyle);
		static bool SetLineStyle(pugi::xml_node node, std::vector<LineStylesPackage::AbstractLineStyle *>* lineStyle);
		static void SetPen(pugi::xml_node node, GraphicBasePackage::Pen* pPen);
		static void SetDash(pugi::xml_node node, LineStylesPackage::Dash* pDash);
		static void SetSymbol(pugi::xml_node node, LineStylesPackage::LineSymbol* pSymbol);
		static void SetCompositeLineStyle(pugi::xml_node node, LineStylesPackage::LineStyle* pLineStyle);
	
	};
}