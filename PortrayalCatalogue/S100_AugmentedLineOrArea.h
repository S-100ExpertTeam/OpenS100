#pragma once
#include "S100_AugmentedGeometry.h"
#include "S100_LineStyleReference.h"
#include "S100_CompositeLineStyle.h"

class S100_AugmentedLineOrArea :
	public S100_AugmentedGeometry
{
public:
	S100_AugmentedLineOrArea();
	virtual ~S100_AugmentedLineOrArea();

private:
	S100_LineStyleReference* lineStyleReference;
	S100_CompositeLineStyle* compositeLineStyle;
	S100_LineStyle *lineStyle;

public:
	void SetLineStyleReference(S100_LineStyleReference* value);
	void SetCompositeLineStyle(S100_CompositeLineStyle* value);
	void SetLineStyle(S100_LineStyle* value);

	S100_LineStyleReference* GetLineStyleReference();
	S100_CompositeLineStyle* GetCompositeLineStyle();
	S100_LineStyle* GetLineStyle(); 
};