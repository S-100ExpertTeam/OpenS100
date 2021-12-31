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
		// LineStyles folder path.
		static bool Open(std::wstring path, LineStylesPackage::LineStyles *pLineStyles);
		static bool OpenByPugi(std::wstring path, LineStylesPackage::LineStyles *pLineStyles);


	private:
		static bool SetLineStyle(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::AbstractLineStyle **pLineStyle);
		static bool SetLineStyle(MSXML2::IXMLDOMNodePtr _pNode, std::vector<LineStylesPackage::AbstractLineStyle *>* lineStyle);
		static void SetPen(MSXML2::IXMLDOMNodePtr _pNode, GraphicBasePackage::Pen* pPen);
		static void SetDash(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::Dash* pDash);
		static void SetSymbol(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::LineSymbol* pSymbol);
		static void SetCompositeLineStyle(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::LineStyle* pLineStyle);
	
	//pugi::change
	private:
		static bool SetLineStyle(pugi::xml_node node, LineStylesPackage::AbstractLineStyle **pLineStyle);
		static bool SetLineStyle(pugi::xml_node node, std::vector<LineStylesPackage::AbstractLineStyle *>* lineStyle);
		static void SetPen(pugi::xml_node node, GraphicBasePackage::Pen* pPen);
		static void SetDash(pugi::xml_node node, LineStylesPackage::Dash* pDash);
		static void SetSymbol(pugi::xml_node node, LineStylesPackage::LineSymbol* pSymbol);
		static void SetCompositeLineStyle(pugi::xml_node node, LineStylesPackage::LineStyle* pLineStyle);
	
	};
}