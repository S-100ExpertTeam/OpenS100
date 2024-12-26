#include "stdafx.h"
#include "DisplayFactory.h"

S100_DisplayList* CDisplayFactory::createDisplayList()
{
	S100_DisplayList* displayList = new S100_DisplayList();

	return displayList;
}

S100_PointInstruction* CDisplayFactory::createPointInstruction(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_PointInstruction* pointInstruction = new S100_PointInstruction();
	if (pointInstruction == nullptr)
		return pointInstruction;

	displayList->SetDisplayInstruction(pointInstruction);

	return pointInstruction;
}

S100_Symbol* CDisplayFactory::createSymbol(S100_PointInstruction* pointInstruction)
{
	if (pointInstruction == nullptr)
		return nullptr;

	S100_Symbol* symbol = new S100_Symbol();
	if (symbol == nullptr)
		return nullptr;

	pointInstruction->SetSymbol(symbol);

	return symbol;
}

S100_VectorPoint* CDisplayFactory::createVectorPoint(S100_PointInstruction* pointInstruction)
{
	if (pointInstruction == nullptr)
		return nullptr;

	S100_VectorPoint* vectorPoint = new S100_VectorPoint();
	if (vectorPoint == nullptr)
		return nullptr;

	pointInstruction->SetVectorPoint(vectorPoint);

	return vectorPoint;
}

S100_LineInstruction* CDisplayFactory::createLineInstruction(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_LineInstruction* lineInstruction = new S100_LineInstruction();
	if (lineInstruction == nullptr)
		return lineInstruction;

	displayList->SetDisplayInstruction(lineInstruction);

	return lineInstruction;
}

S100_LineStyleReference* CDisplayFactory::createLineStyleReference(S100_LineInstruction* lineInstruction)
{
	if (lineInstruction == nullptr)
		return nullptr;

	S100_LineStyleReference* lineStyleReference = new S100_LineStyleReference();
	if (lineStyleReference == nullptr)
		return nullptr;

	lineInstruction->SetLineStyleReference(lineStyleReference);

	return lineStyleReference;
}

S100_LineStyle* CDisplayFactory::createLineStyle(S100_LineInstruction* lineInstruction)
{
	if (lineInstruction == nullptr)
		return nullptr;

	S100_LineStyle* lineStyle = new S100_LineStyle();
	if (lineStyle == nullptr)
		return nullptr;

	lineInstruction->SetLineStyle(lineStyle);

	return lineStyle;
}

S100_AreaInstruction* CDisplayFactory::createAreaInstruction(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_AreaInstruction* areaInstruction = new S100_AreaInstruction();
	if (areaInstruction == nullptr)
		return areaInstruction;

	displayList->SetDisplayInstruction(areaInstruction);

	return areaInstruction;
}

S100_ColorFill* CDisplayFactory::createColorFill(S100_AreaInstruction* areaInstruction)
{
	if (areaInstruction == nullptr)
		return nullptr;

	S100_ColorFill* colorFill = new S100_ColorFill();
	if (colorFill == nullptr)
		return nullptr;

	areaInstruction->SetAreaFill(colorFill);

	return colorFill;
}

S100_Color* CDisplayFactory::createColor(S100_ColorFill* colorFill)
{
	if (colorFill == nullptr)
		return nullptr;

	S100_Color* color = new S100_Color();
	if (color == nullptr)
		return nullptr;

	colorFill->SetColor(color);

	return color;
}

S100_AreaFillReference* CDisplayFactory::createAreaFillReference(S100_AreaInstruction* areaInstruction)
{
	if (areaInstruction == nullptr)
		return nullptr;

	S100_AreaFillReference* areaFillReference = new S100_AreaFillReference();
	if (areaFillReference == nullptr)
		return nullptr;

	areaInstruction->SetAreaFill(areaFillReference);

	return areaFillReference;
}

S100_SpatialReference* CDisplayFactory::createSpatialReference(S100_Instruction* instruction)
{
	if (instruction == nullptr)
		return nullptr;

	S100_SpatialReference* spatialReference = new S100_SpatialReference();
	if (spatialReference == nullptr)
		return nullptr;

	instruction->SetSpatialReference(spatialReference);

	return spatialReference;
}

S100_TextInstruction* CDisplayFactory::createTextInstruction(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_TextInstruction* textInstruction = new S100_TextInstruction();
	if (textInstruction == nullptr)
		return textInstruction;

	displayList->SetDisplayInstruction(textInstruction);

	return textInstruction;
}

S100_TextPoint* CDisplayFactory::createTextPoint(S100_TextInstruction* textInstruction)
{
	if (textInstruction == nullptr)
		return nullptr;

	S100_TextPoint* textPoint = new S100_TextPoint();
	if (textPoint == nullptr)
		return nullptr;

	textInstruction->SetTextPoint(textPoint);

	return textPoint;
}

S100_VectorPoint* CDisplayFactory::createVectorPoint(S100_TextPoint* textPoint)
{
	if (textPoint == nullptr)
		return nullptr;

	S100_VectorPoint* vectorPoint = new S100_VectorPoint();
	if (vectorPoint == nullptr)
		return nullptr;

	textPoint->SetOffset(vectorPoint);

	return vectorPoint;
}

S100_Element* CDisplayFactory::createElement(S100_TextPoint* textPoint)
{
	if (textPoint == nullptr)
		return nullptr;

	S100_Element* element = new S100_Element();
	if (element == nullptr)
		return nullptr;

	textPoint->SetElement(element);

	return element;
}

S100_Text* CDisplayFactory::createText(S100_Element* element)
{
	if (element == nullptr)
		return nullptr;

	S100_Text* text = new S100_Text();
	if (text == nullptr)
		return nullptr;

	element->SetText(text);

	return text;
}

S100_Font* CDisplayFactory::createFont(S100_Element* element)
{
	if (element == nullptr)
		return nullptr;

	S100_Font* font = new S100_Font();
	if (font == nullptr)
		return nullptr;

	element->SetFont(font);

	return font;
}

S100_Foreground* CDisplayFactory::createForeground(S100_Element* element)
{
	if (element == nullptr)
		return nullptr;

	S100_Foreground* foreground = new S100_Foreground();
	if (foreground == nullptr)
		return nullptr;

	element->SetForground(foreground);

	return foreground;
}

S100_AugmentedRay* CDisplayFactory::createAugmentedRay(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_AugmentedRay* augmentedRay = new S100_AugmentedRay();
	if (augmentedRay == nullptr)
		return augmentedRay;

	displayList->SetDisplayInstruction(augmentedRay);

	return augmentedRay;
}

S100_LineStyle* CDisplayFactory::createLineStyle(S100_AugmentedRay* augmentedRay)
{
	if (augmentedRay == nullptr)
		return nullptr;

	S100_LineStyle* lineStyle = new S100_LineStyle();
	if (lineStyle == nullptr)
		return nullptr;

	augmentedRay->SetLineStyle(lineStyle);

	return lineStyle;
}

S100_AugmentedPath* CDisplayFactory::createAugmentedPath(S100_DisplayList* displayList)
{
	if (displayList == nullptr)
		return nullptr;

	S100_AugmentedPath* augmentedPath = new S100_AugmentedPath();
	if (augmentedPath == nullptr)
		return augmentedPath;

	displayList->SetDisplayInstruction(augmentedPath);

	return augmentedPath;
}

S100_LineStyle* CDisplayFactory::createLineStyle(S100_AugmentedPath* augmentedPath)
{
	if (augmentedPath == nullptr)
		return nullptr;

	S100_LineStyle* lineStyle = new S100_LineStyle();
	if (lineStyle == nullptr)
		return nullptr;

	augmentedPath->SetLineStyle(lineStyle);

	return lineStyle;
}

S100_Path* CDisplayFactory::createPath(S100_AugmentedPath* augmentedPath)
{
	if (augmentedPath == nullptr)
		return nullptr;

	S100_Path* path = new S100_Path();
	if (path == nullptr)
		return nullptr;

	augmentedPath->SetPath(path);

	return path;
}

void CDisplayFactory::destroyDisplayList(S100_DisplayList* displayList)
{
	if (displayList)
		delete displayList, displayList = nullptr;

	return;
}

