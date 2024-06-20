#pragma once

#include "S100_SpatialReference.h"
#include "S100_LineStyle.h"

#include "../FeatureCatalog/PrimitiveTypes.h"

#include <string>
#include <optional>
#include <vector>

class PCOutputSchemaManager;
class PortrayalCatalogue;

// DrawingInstruction
class S100_Instruction
{
public:
	S100_Instruction();
	virtual ~S100_Instruction();

private:
	/*
	* Type Of Instruction
	* 0 : Null Instruction
	* 1 : Point Instruction
	* 2 : Line Instruction
	* 3 : Area Instruction
	* 4 : Coverage Instruction
	* 5 : Text Instruction
	* 6 : Augmented Point
	* 7 : Augmented Ray
	* 8 : Augmented Path
	* 9 : Augmented Area
	*/
	int type;

	std::wstring featureReference;
	std::optional<S100::CharacterString> id;
	std::optional<S100::CharacterString> parentId;
	std::optional<S100::Boolean> hover;

	std::list<S100_SpatialReference*> spatialReference;
	std::wstring displayPlane;
	std::wstring drawingPriority;
	std::wstring scaleMinimum;
	std::wstring scaleMaximum;

protected:
	std::vector<std::wstring> viewingGroup;

public:
	void GetContents(pugi::xml_node node);

	void SetType(int value);
	void SetFeatureReference(std::wstring& value);
	void SetSpatialReference(S100_SpatialReference* value);
	void SetSpatialReference(std::list<S100_SpatialReference*> value);
	void SetViewingGroup(std::wstring& value);
	void SetDisplayPlane(std::wstring& value);
	void SetDrawingProiority(std::wstring& value);
	void SetScaleMinimum(std::wstring& value);
	void SetScaleMaximum(std::wstring& value);

	int GetType();
	const std::wstring& GetFeatureReference();
	S100_SpatialReference* GetSpatialReference(int index);
	std::list<S100_SpatialReference*> GetSpatialReference();
	std::wstring GetViewingGroup(int index);
	int getViewingGroupCount();
	std::wstring GetDisplayPlane();
	std::wstring GetDrawingProiority();
	std::wstring GetScaleMinimum();
	std::wstring GetScaleMaximum();
};