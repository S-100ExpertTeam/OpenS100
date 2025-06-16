#include "stdafx.h"
#include "DrawingInstructionReader.h"
#include "TextElement.h"
#include "FontCharacteristics.h"

DrawingInstructionReader::DrawingInstructionReader()
{

}

DrawingInstructionReader::~DrawingInstructionReader()
{

}

PointInstruction* DrawingInstructionReader::GetPointInstructionBypugixml(pugi::xml_node& node)
{
	PointInstruction* result = new PointInstruction();

	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::pointInstruction;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "symbol"))
		{
			result->symbolName = child.attribute("reference").as_string();
			result->m_pSymbol = nullptr;

			result->rotation = (FLOAT)child.attribute("rotation").as_int();
		}
	}

	return result;
}

LineInstruction* DrawingInstructionReader::GetLineInstructionBypugixml(pugi::xml_node& node)
{
	LineInstruction* result = new LineInstruction();
	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::lineInstruction;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "lineStyleReference"))
		{
			result->lineStyleReference = child.attribute("reference").as_string();
		}
		else if (!strcmp(child.name(), "lineStyle"))
		{
			if (child.child("dash"))
			{
				result->hasDash = true;
			}

			if (pugi::xml_node pen = child.child("pen"))
			{
				if (pugi::xml_node color = pen.child("color"))
				{
					result->colorName = color.child_value();
				}

				result->width = pen.attribute("width").as_double(0.32);
			}
		}
		else
		{
			result->width = 0.32;
			result->colorName = _T("CHBLK");
		}
	}

	return result;
}

AreaInstruction* DrawingInstructionReader::GetAreaInstructionBypugixml(pugi::xml_node& node)
{
	AreaInstruction* result = new AreaInstruction();
	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::areaInstruction;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "colorFill"))
		{
			if (pugi::xml_node color = child.first_child())
			{
				result->colorName = color.child_value();

				result->transparency = (FLOAT)color.attribute("transparency").as_double(1.0);
			}
		}
		else if (!strcmp(child.name(), "areaFillReference"))
		{
			result->colorName = _T("CHBLK");
			result->patternName = child.attribute("reference").as_string();
		}
	}

	return result;
}

TextInstruction* DrawingInstructionReader::GetTextInstructionBypugixml(pugi::xml_node& node)
{
	TextInstruction* result = new TextInstruction();
	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::textInstruction;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "textPoint"))
		{
			result->pText = GetText_TextPointBypugixml(child);
		}
		else if (!strcmp(child.name(), "TextLine"))
		{
			// empty
		}
	}

	return result;
}

AugmentedPath* DrawingInstructionReader::GetAugmentedPathInstructionBypugixml(pugi::xml_node& node)
{
	AugmentedPath* result = new AugmentedPath();

	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::augmentedPath;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "viewingGroup"))
		{
		}
		else if (!strcmp(child.name(), "displayPlane"))
		{
		}
		else if (!strcmp(child.name(), "drawingPriority"))
		{
		}
		else if (!strcmp(child.name(), "crs"))
		{
			result->_crs = GetGraphicBase_CRSTypeBypugixml(child);
		}
		else if (!strcmp(child.name(), "textPoint"))
		{
			result->_text = GetText_TextPointBypugixml(child);
		}
		else if (!strcmp(child.name(), "textLine"))
		{
		}
		else if (!strcmp(child.name(), "lineStyle"))
		{
			result->pLineStyle = GetLineStyles_LineStyleBypugixml(child);
		}
		else if (!strcmp(child.name(), "lineStyleReference"))
		{
			result->pLineStyle = GetLineStyles_LineStyleReferenceBypugixml(child);
		}
		else if (!strcmp(child.name(), "compositeLineStyle"))
		{
			result->pLineStyle = GetLineStyles_CompositeLineStyleBypugixml(child);
		}
		else if (!strcmp(child.name(), "path"))
		{
			GraphicBasePackage::Path* pPath = GetGraphicBase_Path(child);
			result->_path = *pPath;
			pPath->_segment.clear();
			delete pPath;
			pPath = nullptr;
		}
		else
		{
			if (OUTPUT_DEBUG_STRING)
			{
				CString strErr;
				strErr.Format(_T("Invalid child of AugmentedPath : %S\n"), child.name());
				//OutputDebugString(strErr);
			}
		}
	}

	return result;
}

AugmentedRay* DrawingInstructionReader::GetAugmentedRayInstructionBypugixml(pugi::xml_node& node)
{
	AugmentedRay* result = new AugmentedRay();

	result->m_nTypeOfDrawingInstruction = TypeOfDrawingInstruction::augmentedRay;

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = child.child_value();
			result->m_pFeatureReference = featureReference;
		}
		else if (!strcmp(child.name(), "viewingGroup"))
		{
		}
		else if (!strcmp(child.name(), "displayPlane"))
		{
		}
		else if (!strcmp(child.name(), "drawingPriority"))
		{
		}
		else if (!strcmp(child.name(), "crs"))
		{
			result->_crs = GetGraphicBase_CRSTypeBypugixml(child);
		}
		else if (!strcmp(child.name(), "lineStyle"))
		{
			result->pLineStyle = GetLineStyles_LineStyleBypugixml(child);
		}
		else if (!strcmp(child.name(), "lineStyleReference"))
		{
			result->pLineStyle = GetLineStyles_LineStyleReferenceBypugixml(child);
		}
		else if (!strcmp(child.name(), "compositeLineStyle"))
		{
			result->pLineStyle = GetLineStyles_CompositeLineStyleBypugixml(child);
		}
		else if (!strcmp(child.name(), "rotationCRS"))
		{
			result->_rotationCRS = new GraphicBasePackage::CRSType;
			*result->_rotationCRS = GetGraphicBase_CRSTypeBypugixml(child);
		}
		else if (!strcmp(child.name(), "direction"))
		{
			result->dDirection = atof(child.child_value());
		}
		else if (!strcmp(child.name(), "length"))
		{
			result->dLength = atof(child.child_value());
		}
		else
		{
			if (OUTPUT_DEBUG_STRING)
			{
				CString strErr;
				strErr.Format(_T("Invalid child of AugmentedPath : %S\n"), child.name());
				//OutputDebugString(strErr);
			}
		}
	}

	return result;
}

GraphicBasePackage::CRSType DrawingInstructionReader::GetGraphicBase_CRSTypeBypugixml(pugi::xml_node& node)
{
	const pugi::char_t* value = node.child_value();
	if (!strcmp(value, "GeographicCRS"))
	{
		return GraphicBasePackage::CRSType::geographicCRS;
	}
	else if (!strcmp(value, "PortrayalCRS"))
	{
		return GraphicBasePackage::CRSType::portrayalCRS;
	}
	else if (!strcmp(value, "LocalCRS"))
	{
		return GraphicBasePackage::CRSType::localCRS;
	}
	else if (!strcmp(value, "LineCRS"))
	{
		return GraphicBasePackage::CRSType::lineCRS;
	}
	else
	{
		if (OUTPUT_DEBUG_STRING)
		{
			CString strErr;
			strErr.Format(_T("Invalid value of CRSType : %S\n"), value);
			//OutputDebugString(strErr);
		}
	}

	return GraphicBasePackage::CRSType::CRSType_None;
}

TextPackage::TextPoint* DrawingInstructionReader::GetText_TextPointBypugixml(pugi::xml_node& node)
{
	TextPackage::TextPoint* pTextPoint = new TextPackage::TextPoint();
	GetText_TextBypugixml(node, pTextPoint);
	
	if (pugi::xml_node element = node.child("offset"))
	{
		pTextPoint->offset = new GraphicBasePackage::Vector();
		pTextPoint->offset->x = atof(element.child_value("x"));
		pTextPoint->offset->y = atof(element.child_value("y"));
	}

	if (pugi::xml_node element = node.child("areaPlacement"))
	{

	}

	return pTextPoint;
}

TextPackage::TextLine* DrawingInstructionReader::GetText_TextLineBypugixml(pugi::xml_node& node)
{
	return nullptr;
}

void DrawingInstructionReader::GetText_TextBypugixml(pugi::xml_node& textNode, TextPackage::Text* pText)
{
	const pugi::char_t* value_HA = textNode.attribute("horizontalAlignment").as_string();

	if (!strcmp(value_HA, "Start"))
	{
		pText->horizontalAlignment = TextPackage::HorizontalAlignment::start;
	}
	else if (!strcmp(value_HA, "End"))
	{
		pText->horizontalAlignment = TextPackage::HorizontalAlignment::end;
	}
	else if (!strcmp(value_HA, "Center"))
	{
		pText->horizontalAlignment = TextPackage::HorizontalAlignment::h_center;
	}

	const pugi::char_t* value_VA = textNode.attribute("verticalAlignment").as_string();

	if (!strcmp(value_VA, "Top"))
	{
		pText->verticalAlignment = TextPackage::VerticalAlignment::top;
	}
	else if (!strcmp(value_VA, "Bottom"))
	{
		pText->verticalAlignment = TextPackage::VerticalAlignment::bottom;
	}
	else if (!strcmp(value_VA, "Center"))
	{
		pText->verticalAlignment = TextPackage::VerticalAlignment::v_center;
	}

	pugi::xml_node element = textNode.child("element");
	while (element)
	{
		TextPackage::TextElement* textElement = new TextPackage::TextElement();
		pText->element.push_back(textElement);
		wchar_t* wText = LibMFCUtil::ConvertCtoWC((char*)element.child_value("text"));
		textElement->text = wText;
		delete[] wText;
		textElement->bodySize = atoi(element.child_value("bodySize"));
		textElement->verticalOffset = atof(element.child_value("verticalOffset"));
		// flags
		// foreground
		// background

		if (pugi::xml_node fontCharacteristic = element.child("fontCharacteristic"))
		{
			TextPackage::FontCharacteristics* fc = new TextPackage::FontCharacteristics();

			if (!strcmp(fontCharacteristic.child_value("weight"), "light"))
			{
				fc->weight = TextPackage::FontWeight::light;
			}
			else if (!strcmp(fontCharacteristic.child_value("weight"), "medium"))
			{
				fc->weight = TextPackage::FontWeight::medium;
			}
			else if (!strcmp(fontCharacteristic.child_value("weight"), "bold"))
			{
				fc->weight = TextPackage::FontWeight::bold;
			}

			if (!strcmp(fontCharacteristic.child_value("slant"), "upright"))
			{
				fc->slant = TextPackage::FontSlant::upright;
			}
			else if (!strcmp(fontCharacteristic.child_value("slant"), "italics"))
			{
				fc->slant = TextPackage::FontSlant::italics;
			}

			// serifs
			// proportion
			textElement->font = (TextPackage::Font*)fc;
		}

		element = element.next_sibling();
	}
}

LineStylesPackage::LineStyle* DrawingInstructionReader::GetLineStyles_LineStyleBypugixml(pugi::xml_node& node)
{
	LineStylesPackage::LineStyle* pLineStyle = new LineStylesPackage::LineStyle();

	if (pugi::xml_node intervalLengthNode = node.child("intervalLength"))
	{
		pLineStyle->intervalLength = atof(intervalLengthNode.child_value());
	}
	else if (pugi::xml_node penNode = node.child("pen"))
	{
		pLineStyle->pen = new GraphicBasePackage::Pen();
		GetGraphicBase_PenBypugixml(penNode, pLineStyle->pen);
	}
	else if (pugi::xml_node dashNode = node.child("dash"))
	{
		LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
		GetLineStyles_DashBypugixml(dashNode, pDash);
		pLineStyle->dash.push_back(pDash);
	}
	else if (pugi::xml_node symbolNode = node.child("symbol"))
	{
		LineStylesPackage::LineSymbol* pLineSymbol = new LineStylesPackage::LineSymbol();
		GetLineStyles_LineSymbolBypugixml(symbolNode, pLineSymbol);
		pLineStyle->symbol.push_back(pLineSymbol);
	}

	pLineStyle->_capStyle = GetLineStyles_CapStyle(node.attribute("capStyle").as_string());
	pLineStyle->_joinStyle = GetLineStyles_JoinStyle(node.attribute("joinStyle").as_string("Miter"));
	pLineStyle->offset = node.attribute("offset").as_double(0);

	return pLineStyle;
}

LineStylesPackage::LineStyleReference* DrawingInstructionReader::GetLineStyles_LineStyleReferenceBypugixml(pugi::xml_node& node)
{
	LineStylesPackage::LineStyleReference* pLineStyleReference = new LineStylesPackage::LineStyleReference();
	
	wchar_t* wReference = LibMFCUtil::ConvertCtoWC((char*)node.attribute("reference").as_string());
	pLineStyleReference->reference = wReference;
	delete[] wReference;

	return pLineStyleReference;
}

LineStylesPackage::CompositeLineStyle* DrawingInstructionReader::GetLineStyles_CompositeLineStyleBypugixml(pugi::xml_node& node)
{
	LineStylesPackage::CompositeLineStyle* pCompositeLineStyle = new LineStylesPackage::CompositeLineStyle();

	for (pugi::xml_node lineStyleNode = node.first_child(); lineStyleNode; lineStyleNode = lineStyleNode.next_sibling())
	{
		const pugi::char_t* name = lineStyleNode.name();

		if (!strcmp(name, "lineStyle"))
		{
			LineStylesPackage::LineStyle* pLineStyle = new LineStylesPackage::LineStyle();
			pCompositeLineStyle->component.push_back(pLineStyle);
		}
		else if (!strcmp(name, "lineStyleReference"))
		{
			LineStylesPackage::LineStyleReference* pLineStyleReference = new LineStylesPackage::LineStyleReference();
			pCompositeLineStyle->component.push_back(pLineStyleReference);
		}
		else if (!strcmp(name, "compositeLineStyle"))
		{
			LineStylesPackage::CompositeLineStyle* pCompositeLineStyle = new LineStylesPackage::CompositeLineStyle();
			pCompositeLineStyle->component.push_back(pCompositeLineStyle);
		}
	}

	return pCompositeLineStyle;
}

GraphicBasePackage::Pen* DrawingInstructionReader::GetGraphicBase_PenBypugixml(pugi::xml_node& penNode)
{
	GraphicBasePackage::Pen* pPen = new GraphicBasePackage::Pen();
	GetGraphicBase_PenBypugixml(penNode, pPen);
	return pPen;
}

void DrawingInstructionReader::GetGraphicBase_PenBypugixml(pugi::xml_node& penNode, GraphicBasePackage::Pen* pPen)
{
	if (pugi::xml_node colorNode = penNode.child("color"))
	{
		wchar_t* wToken = LibMFCUtil::ConvertCtoWC((char*)colorNode.child_value());
		pPen->color.SetToken(wToken);
		delete[] wToken;
		wToken = nullptr;

		pPen->color.SetTransparency(colorNode.attribute("transparency").as_double(0));
	}

	pPen->width = penNode.attribute("width").as_double(0.32);
}

LineStylesPackage::Dash* DrawingInstructionReader::GetLineStyles_DashBypugixml(pugi::xml_node& dashNode)
{
	LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
	GetLineStyles_DashBypugixml(dashNode, pDash);
	return pDash;
}

void DrawingInstructionReader::GetLineStyles_DashBypugixml(pugi::xml_node& dashNode, LineStylesPackage::Dash* pDash)
{
	if (pugi::xml_node startNode = dashNode.child("start"))
	{
		pDash->start = atof(startNode.child_value());
	}

	if (pugi::xml_node lengthNode = dashNode.child("length"))
	{
		pDash->length = atof(lengthNode.child_value());
	}
}

LineStylesPackage::LineSymbol* DrawingInstructionReader::GetLineStyles_LineSymbolBypugixml(pugi::xml_node& symbolNode)
{
	LineStylesPackage::LineSymbol* pLineSymbol = new LineStylesPackage::LineSymbol();
	GetLineStyles_LineSymbolBypugixml(symbolNode, pLineSymbol);
	return pLineSymbol;
}

void DrawingInstructionReader::GetLineStyles_LineSymbolBypugixml(pugi::xml_node& symbolNode, LineStylesPackage::LineSymbol* pLineSymbol)
{
	if (pugi::xml_node positionNode = symbolNode.child("position"))
	{
		pLineSymbol->position = atof(positionNode.child_value());
	}

	wchar_t* wReference = LibMFCUtil::ConvertCtoWC((char*)symbolNode.attribute("reference").as_string());
	pLineSymbol->reference = wReference;
	delete[] wReference;
	wReference = nullptr;

	pLineSymbol->_rotation = symbolNode.attribute("rotation").as_double(0);

	pLineSymbol->_scaleFactor = symbolNode.attribute("scaleFactor").as_double(1);

	pugi::xml_node crsTypeNode = symbolNode.child("srcType");
	if (!crsTypeNode.empty())
		pLineSymbol->_crsType = GetGraphicBase_CRSTypeBypugixml(crsTypeNode);
}

LineStylesPackage::CapStyle DrawingInstructionReader::GetLineStyles_CapStylebypugixml(pugi::xml_node& capStyleNode)
{
	const pugi::char_t* value = capStyleNode.child_value();

	return GetLineStyles_CapStyle(value);
}

LineStylesPackage::CapStyle DrawingInstructionReader::GetLineStyles_CapStyle(const char* value)
{
	if (!strcmp(value, "Butt"))
	{
		return LineStylesPackage::CapStyle::eCapStyle_butt;
	}
	else if (!strcmp(value, "Square"))
	{
		return LineStylesPackage::CapStyle::eCapStyle_square;
	}
	else if (!strcmp(value, "Round"))
	{
		return LineStylesPackage::CapStyle::eCapStyle_round;
	}

	return LineStylesPackage::CapStyle::eCapStyle_butt;
}

LineStylesPackage::JoinStyle DrawingInstructionReader::GetLineStyles_JoinStyle(const char* value)
{
	if (!strcmp(value, "Bevel"))
	{
		return LineStylesPackage::JoinStyle::eJoinStyle_bevel;
	}
	else if (!strcmp(value, "Miter"))
	{
		return LineStylesPackage::JoinStyle::eJoinStyle_miter;
	}
	else if (!strcmp(value, "Round"))
	{
		return LineStylesPackage::JoinStyle::eJoinStyle_round;
	}

	return LineStylesPackage::JoinStyle::eJoinStyle_miter;
}

GraphicBasePackage::Path* DrawingInstructionReader::GetGraphicBase_Path(pugi::xml_node& pathNode)
{
	GraphicBasePackage::Path* pPath = new GraphicBasePackage::Path();

	if (pugi::xml_node polylineNode = pathNode.child("polyline"))
	{
		GraphicBasePackage::Polyline* pPolyline = GetGraphicBase_Polyline(polylineNode);
		pPath->_segment.push_back(pPolyline);
	}
	else if (pugi::xml_node arc3PointsNode = pathNode.child("arc3Points"))
	{
		GraphicBasePackage::Arc3Points* pArc3Points = GetGraphicBase_Arc3Points(arc3PointsNode);
		pPath->_segment.push_back(pArc3Points);
	}
	else if (pugi::xml_node arcByRadiusNode = pathNode.child("arcByRadius"))
	{
		GraphicBasePackage::ArcByRadius* pArcByRadius = GetGraphicBase_ArcByRadius(arcByRadiusNode);
		pPath->_segment.push_back(pArcByRadius);
	}
	else if (pugi::xml_node annulusNode = pathNode.child("annulus"))
	{
		GraphicBasePackage::Annulus* pAnnulus = GetGraphicBase_Annulus(annulusNode);
		pPath->_segment.push_back(pAnnulus);
	}
	else
	{
		if (OUTPUT_DEBUG_STRING)
		{
			//OutputDebugString(_T("Path node has invalid child or hasn't child\n"));
		}
	}

	return pPath;
}

GraphicBasePackage::Polyline* DrawingInstructionReader::GetGraphicBase_Polyline(pugi::xml_node& polylineNode)
{
	GraphicBasePackage::Polyline* pPolyline = new GraphicBasePackage::Polyline();

	for (pugi::xml_node child = polylineNode.child("point"); child; child = child.next_sibling())
	{
		pPolyline->_point.push_back(GetGraphicBase_Point(child));
	}

	return pPolyline;
}

GraphicBasePackage::Arc3Points* DrawingInstructionReader::GetGraphicBase_Arc3Points(pugi::xml_node& arc3PointsNode)
{
	GraphicBasePackage::Arc3Points* pArc3Points = new GraphicBasePackage::Arc3Points();

	for (pugi::xml_node child = arc3PointsNode.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "startPoint"))
		{
			pArc3Points->_startPoint = GetGraphicBase_Point(child);
		}
		else if (!strcmp(child.name(), "medianPoint"))
		{
			pArc3Points->_medianPoint = GetGraphicBase_Point(child);
		}
		else if (!strcmp(child.name(), "endPoint"))
		{
			pArc3Points->_endPoint = GetGraphicBase_Point(child);
		}
	}

	return pArc3Points;
}

GraphicBasePackage::ArcByRadius* DrawingInstructionReader::GetGraphicBase_ArcByRadius(pugi::xml_node& arcByRadiusNode)
{
	GraphicBasePackage::ArcByRadius* pArcByRadius = new GraphicBasePackage::ArcByRadius();

	for (pugi::xml_node child = arcByRadiusNode.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "center"))
		{
			pArcByRadius->_center = GetGraphicBase_Point(child);
		}
		else if (!strcmp(child.name(), "sector"))
		{
			pArcByRadius->_sector = new GraphicBasePackage::Sector();
			*pArcByRadius->_sector = GetGraphicBase_Sector(child);
		}
		else if (!strcmp(child.name(), "radius"))
		{
			pArcByRadius->_radius = atof(child.child_value());
		}
	}

	return pArcByRadius;
}

GraphicBasePackage::Annulus* DrawingInstructionReader::GetGraphicBase_Annulus(pugi::xml_node& annulusNode)
{
	GraphicBasePackage::Annulus* pAnnulus = new GraphicBasePackage::Annulus();

	for (pugi::xml_node child = annulusNode.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "center"))
		{
			pAnnulus->_center = GetGraphicBase_Point(child);
		}
		else if (!strcmp(child.name(), "innerRadius"))
		{
			pAnnulus->_innerRadius = new double;
			*pAnnulus->_innerRadius = atof(child.child_value());
		}
		else if (!strcmp(child.name(), "outerRadius"))
		{
			pAnnulus->_outerRadius = atof(child.child_value());
		}
		else if (!strcmp(child.name(), "outerRadius"))
		{
		}
		else
		{
			if (OUTPUT_DEBUG_STRING)
			{
				CString strErr;
				strErr.Format(_T("Invalid child of Annulus : %S\n"), child.name());
				//OutputDebugString(strErr);
			}
		}
	}

	return pAnnulus;
}

GraphicBasePackage::Point DrawingInstructionReader::GetGraphicBase_Point(pugi::xml_node& pointNode)
{
	GraphicBasePackage::Point point;

	point.x = atof(pointNode.child_value("x"));
	point.y = atof(pointNode.child_value("y"));

	return point;
}

GraphicBasePackage::Sector DrawingInstructionReader::GetGraphicBase_Sector(pugi::xml_node& sectorNode)
{
	GraphicBasePackage::Sector sector;

	for (pugi::xml_node child = sectorNode.first_child(); child; child = child.next_sibling())
	{
		if (!strcmp(child.name(), "rotationCRS"))
		{
			sector._rotationCRS = GetGraphicBase_CRSTypeBypugixml(child);
		}
		else if (!strcmp(child.name(), "startAngle"))
		{
			sector._startAngle = atof((child.child_value()));
		}
		else if (!strcmp(child.name(), "angularDistance"))
		{
			sector._angularDistance = atof((child.child_value()));
		}
	}
	return sector;
}

DisplayList* DrawingInstructionReader::OpenOutputXMLbypugixml(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
		return nullptr;

	pugi::xml_node displayList = doc.child("displayList");

	if (!displayList)
		return nullptr;

	DisplayList* pDisplayList = new DisplayList();

	for (pugi::xml_node instruction = displayList.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "pointInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetPointInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "lineInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetLineInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "areaInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAreaInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "textInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetTextInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "augmentedPath"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAugmentedPathInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "augmentedRay"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAugmentedRayInstructionBypugixml(instruction));
		}
	}

	return pDisplayList;
}