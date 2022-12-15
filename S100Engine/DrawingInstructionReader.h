#pragma once
#include "PointInstruction.h"
#include "LineInstruction.h"
#include "AreaInstruction.h"
#include "TextInstruction.h"
#include "AugmentedPath.h"
#include "AugmentedRay.h"
#include "Text.h"
#include "LineStylesPackage.h"
#include "TextPoint.h"
#include "TextLine.h"

#include <pugixml.hpp>

class DrawingInstructionReader
{
public:
	DrawingInstructionReader();
	virtual ~DrawingInstructionReader();

public:
	static PointInstruction* GetPointInstructionBypugixml(pugi::xml_node& node);
	static LineInstruction* GetLineInstructionBypugixml(pugi::xml_node& node);
	static AreaInstruction* GetAreaInstructionBypugixml(pugi::xml_node& node);
	static TextInstruction* GetTextInstructionBypugixml(pugi::xml_node& node);
	static AugmentedPath* GetAugmentedPathInstructionBypugixml(pugi::xml_node& node);
	static AugmentedRay* GetAugmentedRayInstructionBypugixml(pugi::xml_node& node);

	static GraphicBasePackage::CRSType GetGraphicBase_CRSTypeBypugixml(pugi::xml_node& node);
	static TextPackage::TextPoint* GetText_TextPointBypugixml(pugi::xml_node& node);
	static TextPackage::TextLine* GetText_TextLineBypugixml(pugi::xml_node& node);
	static void GetText_TextBypugixml(pugi::xml_node& textNode, TextPackage::Text* pText);

	static LineStylesPackage::LineStyle* GetLineStyles_LineStyleBypugixml(pugi::xml_node& node);
	static LineStylesPackage::LineStyleReference* GetLineStyles_LineStyleReferenceBypugixml(pugi::xml_node& node);
	static LineStylesPackage::CompositeLineStyle* GetLineStyles_CompositeLineStyleBypugixml(pugi::xml_node& node);

	static GraphicBasePackage::Pen* GetGraphicBase_PenBypugixml(pugi::xml_node& penNode);
	static void GetGraphicBase_PenBypugixml(pugi::xml_node& penNode, GraphicBasePackage::Pen* pPen);

	static LineStylesPackage::Dash* GetLineStyles_DashBypugixml(pugi::xml_node& dashNode);
	static void GetLineStyles_DashBypugixml(pugi::xml_node& dashNode, LineStylesPackage::Dash* pDash);

	static LineStylesPackage::LineSymbol* GetLineStyles_LineSymbolBypugixml(pugi::xml_node& symbolNode);
	static void GetLineStyles_LineSymbolBypugixml(pugi::xml_node& symbolNode, LineStylesPackage::LineSymbol* pLineSymbol);

	static LineStylesPackage::CapStyle GetLineStyles_CapStylebypugixml(pugi::xml_node& capStyleNode);
	static LineStylesPackage::CapStyle GetLineStyles_CapStyle(const char* value);
	static LineStylesPackage::JoinStyle GetLineStyles_JoinStyle(const char* value);

	static GraphicBasePackage::Path* GetGraphicBase_Path(pugi::xml_node& pathNode);
	static GraphicBasePackage::Polyline* GetGraphicBase_Polyline(pugi::xml_node& polylineNode);
	static GraphicBasePackage::Arc3Points* GetGraphicBase_Arc3Points(pugi::xml_node& arc3PointsNode);
	static GraphicBasePackage::ArcByRadius* GetGraphicBase_ArcByRadius(pugi::xml_node& arcByRadiusNode);
	static GraphicBasePackage::Annulus* GetGraphicBase_Annulus(pugi::xml_node& annulusNode);
	static GraphicBasePackage::Point GetGraphicBase_Point(pugi::xml_node& pointNode);
	static GraphicBasePackage::Sector GetGraphicBase_Sector(pugi::xml_node& sectorNode);
};