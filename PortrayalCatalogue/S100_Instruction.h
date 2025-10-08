#pragma once

#include "S100_SpatialReference.h"
#include "S100_LineStyle.h"
#include "S100_AlertReference.h"

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

protected:
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
	std::vector<std::wstring> viewingGroup;
	std::list<S100_SpatialReference*> spatialReference;
	std::wstring displayPlane;
	S100::Integer drawingPriority = 0;
	std::optional<S100::Integer> scaleMinimum = 0;
	std::optional<S100::Integer> scaleMaximum = 0;
	std::optional<S100_AlertReference*> alertReference;


public:
	void GetContents(pugi::xml_node node);

	void SetType(int value);
	void SetFeatureReference(std::wstring& value);

	void setId(std::string value);
	void setParentId(std::string value);
	void setHover(bool value);

	void SetSpatialReference(S100_SpatialReference* value);
	void SetSpatialReference(std::string& type, std::string& reference);
	void SetSpatialReference(std::list<S100_SpatialReference*> value);
	void SetViewingGroup(std::wstring& value);
	void SetViewingGroup(std::vector<std::string>& value);
	void SetDisplayPlane(std::wstring& value);
	void SetDisplayPlane(std::string& value);
	void SetDrawingPriority(std::wstring& value);
	void SetDrawingPriority(int value);
	void SetScaleMinimum(std::wstring& value);
	void SetScaleMinimum(int value);
	void SetScaleMaximum(std::wstring& value);
	void SetScaleMaximum(int value);
	void setAlertReference(S100_AlertReference* value);

	int GetType();
	const std::wstring& GetFeatureReference();
	const std::string getId();
	const std::string getParentId();
	const bool getHover();
	S100_SpatialReference* GetSpatialReference(int index);
	std::list<S100_SpatialReference*> GetSpatialReference();
	std::wstring GetViewingGroup(int index);
	int getViewingGroupCount();
	std::wstring GetDisplayPlane();
	int GetDrawingProiority();
	int GetScaleMinimum();
	int GetScaleMaximum();
	bool hasAlertReference();
	S100_AlertReference* getAlertReference();
};