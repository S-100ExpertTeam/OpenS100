#include "stdafx.h"
#include "S100_AugmentedLineOrArea.h"

S100_AugmentedLineOrArea::S100_AugmentedLineOrArea()
{
	lineStyleReference = NULL;
	compositeLineStyle = NULL;
	lineStyle = NULL;
}


S100_AugmentedLineOrArea::~S100_AugmentedLineOrArea()
{
	if (lineStyleReference)
		delete lineStyleReference;
	lineStyleReference = NULL;

	if (compositeLineStyle)
		delete compositeLineStyle;
	compositeLineStyle = NULL;

	if (lineStyle)
		delete lineStyle;
	lineStyle = NULL;

}

void S100_AugmentedLineOrArea::SetLineStyleReference(S100_LineStyleReference* value) 
{
	lineStyleReference = value;
}
void S100_AugmentedLineOrArea::SetCompositeLineStyle(S100_CompositeLineStyle* value) 
{
	compositeLineStyle = value;
}
void S100_AugmentedLineOrArea::SetLineStyle(S100_LineStyle* value) 
{
	lineStyle = value;
}

S100_LineStyleReference* S100_AugmentedLineOrArea::GetLineStyleReference()
{
	return lineStyleReference;
}

S100_CompositeLineStyle* S100_AugmentedLineOrArea::GetCompositeLineStyle()
{
	return compositeLineStyle;
}

S100_LineStyle* S100_AugmentedLineOrArea::GetLineStyle()
{
	return lineStyle;
}