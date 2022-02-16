#pragma once
#include "S100_VectorPoint.h"
#include "S100_LineStyle.h"
#include "S100_LineStyleReference.h"
#include "S100_CompositeLineStyle.h"

class S100_Hatch
{
public:
	S100_Hatch();
	virtual ~S100_Hatch();

	// element
private:
	// Group 
	S100_LineStyle lineStyle;
	S100_LineStyleReference lineStyleReference;
	S100_CompositeLineStyle compositeLineStyle;

	S100_VectorPoint direction;
	std::wstring distance;

public:
	void GetContents(pugi::xml_node node);

	void SetLineStyle(S100_LineStyle value);
	S100_LineStyle GetLineStyle();

	void SetLineStyleReference(S100_LineStyleReference* value);
	S100_LineStyleReference* GetLineStyleReference();

	void SetCompositeLineStyle(S100_CompositeLineStyle* value);
	S100_CompositeLineStyle* GetCompositeLineStyle();

	void SetDirection(S100_VectorPoint value);
	S100_VectorPoint GetDirection();

	void SetDistance(std::wstring& value);
	std::wstring GetDistance();
};