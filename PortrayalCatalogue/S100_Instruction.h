#pragma once

#include "S100_FeatureReference.h"
#include "S100_SpatialReference.h"
#include "S100_LineStyle.h"

#include <string>

class PCOutputSchemaManager;
class PortrayalCatalogue;
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

	S100_FeatureReference featureReference;
	std::list<S100_SpatialReference*> spatialReference;
	std::wstring viewingGroup;
	std::wstring displayPlane;
	std::wstring drawingPriority;
	std::wstring scaleMinimum;
	std::wstring scaleMaximum;

public:
	void GetContents(pugi::xml_node node);


	void SetType(int value);
	void SetFeatureReference(S100_FeatureReference* value);
	void SetSpatialReference(S100_SpatialReference* value);
	void SetSpatialReference(std::list<S100_SpatialReference*> value);
	void SetViewingGroup(std::wstring& value);
	void SetDisplayPlane(std::wstring& value);
	void SetDrawingProiority(std::wstring& value);
	void SetScaleMinimum(std::wstring& value);
	void SetScaleMaximum(std::wstring& value);

	int GetType();
	S100_FeatureReference* GetFeatureReference();
	S100_SpatialReference* GetSpatialReference(int index);
	std::list<S100_SpatialReference*> GetSpatialReference();
	std::wstring GetViewingGroup();
	std::wstring GetDisplayPlane();
	std::wstring GetDrawingProiority();
	std::wstring GetScaleMinimum();
	std::wstring GetScaleMaximum();
};