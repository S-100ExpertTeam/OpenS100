#pragma once

#include "S100_DisplayList.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_ColorFill.h"
#include "S100_AugmentedRay.h"
#include "S100_AugmentedPath.h"
#include "S100_AreaFillReference.h"

class CDisplayFactory
{
public:
	static S100_DisplayList* createDisplayList();

	static S100_PointInstruction* createPointInstruction(S100_DisplayList*);
	static S100_Symbol* createSymbol(S100_PointInstruction*);
	static S100_VectorPoint* createVectorPoint(S100_PointInstruction*);
	
	static S100_LineInstruction* createLineInstruction(S100_DisplayList*);
	static S100_LineStyleReference* createLineStyleReference(S100_LineInstruction*);
	static S100_LineStyle* createLineStyle(S100_LineInstruction*);

	static S100_AreaInstruction* createAreaInstruction(S100_DisplayList*);
	static S100_ColorFill* createColorFill(S100_AreaInstruction*);
	static S100_Color* createColor(S100_ColorFill*);
	static S100_AreaFillReference* createAreaFillReference(S100_AreaInstruction*);

	static S100_SpatialReference* createSpatialReference(S100_Instruction*);

	static S100_TextInstruction* createTextInstruction(S100_DisplayList*);
	static S100_TextPoint* createTextPoint(S100_TextInstruction*);
	static S100_VectorPoint* createVectorPoint(S100_TextPoint*);
	static S100_Element* createElement(S100_TextPoint*);
	static S100_Text* createText(S100_Element*);
	static S100_Font* createFont(S100_Element*);
	static S100_Foreground* createForeground(S100_Element*);

	static S100_AugmentedRay* createAugmentedRay(S100_DisplayList*);
	static S100_LineStyle* createLineStyle(S100_AugmentedRay*);

	static S100_AugmentedPath* createAugmentedPath(S100_DisplayList*);
	static S100_LineStyle* createLineStyle(S100_AugmentedPath*);
	static S100_Path* createPath(S100_AugmentedPath*);

	static void destroyDisplayList(S100_DisplayList*);
};

