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
	std::string horizontalAlignment;
	std::string verticalAlignment;
	std::vector<S100_Element*> elements;
	S100_VectorPoint *offset;
	std::string rotation;
	S100_AreaPlacement *areaPlacement;

public:
	void GetContents(pugi::xml_node node);

	void SetHorizontalAlignment(const std::string& value);
	void SetHorizontalAlignment(const std::wstring& value);
	void SetVerticalAlignment(const std::string& value);
	void SetVerticalAlignment(const std::wstring& value);
	void SetElement(S100_Element* value);
	void SetElements(std::vector<S100_Element*> value);
	void SetOffset(S100_VectorPoint* value);
	void SetRotation(const std::string& value);
	void SetRotation(const std::wstring& value);

	std::string  GetHorizontalAlignment();
	std::wstring GetHorizontalAlignmentW();
	std::string  GetVerticalAlignment();
	std::wstring GetVerticalAlignmentW();
	S100_Element* GetElement(int index);
	std::vector<S100_Element*> GetElemets();
	S100_VectorPoint* GetOffset();
	std::string  GetRotation();
	std::wstring GetRotationW();
	S100_AreaPlacement* GetAreaPlacement();
};
