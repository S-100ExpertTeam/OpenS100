#pragma once
#include "S100_Element.h"
#include "S100_AreaPlacement.h"
#include "S100_VectorPoint.h"

#include <vector>

class S100_TextPoint
{
public:
	S100_TextPoint();
	virtual ~S100_TextPoint();

private:
	std::wstring horizontalAlignment;
	std::wstring verticalAlignment;
	std::vector<S100_Element*> elements;
	S100_VectorPoint *offset;
	std::wstring rotation;
	S100_AreaPlacement *areaPlacement;

public:
	void GetContents(pugi::xml_node node);

	void SetHorizontalAlignment(std::wstring& value);
	void SetVerticalAlignment(std::wstring& value);
	void SetElement(S100_Element* value);
	void SetElements(std::vector<S100_Element*> value);
	void SetOffset(S100_VectorPoint* value);
	void SetRotation(std::wstring value);
	void SetAreaPlacement(S100_AreaPlacement* value);

	std::wstring GetHorizontalAlignment();
	std::wstring GetVerticalAlignment();
	S100_Element* GetElement(int index);
	std::vector<S100_Element*> GetElemets();
	S100_VectorPoint* GetOffset();
	std::wstring GetRotation();
	S100_AreaPlacement* GetAreaPlacement();
};