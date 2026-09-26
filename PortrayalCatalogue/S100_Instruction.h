#pragma once

#include "S100_SpatialReference.h"
#include "S100_LineStyle.h"
#include "S100_AlertReference.h"

#include "..\\FeatureCatalog\\PrimitiveTypes.h"
#include "..\\FeatureCatalog\\S100_TM_Period.h"

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

	std::optional<S100::CharacterString> id;
	std::optional<S100::CharacterString> parentId;
	std::optional<S100::Boolean> hover;
	std::vector<std::string> viewingGroup;
	std::string displayPlane;
	S100::Integer drawingPriority = 0;
	std::optional<S100::Integer> scaleMinimum = 0;
	std::optional<S100::Integer> scaleMaximum = 0;
	std::string featureReference;
	std::list<S100_SpatialReference*> spatialReference;
	std::optional<S100_AlertReference> alertReference;
	std::vector<S100_TM_Period> timeValid;

public:
	void GetContents(pugi::xml_node node);

	void SetType(int value);
	void SetFeatureReference(const std::string& value);
	void SetFeatureReference(const std::wstring& value);
	void SetFeatureReference(std::string_view value);

	void setId(std::string value);
	void setParentId(std::string value);
	void setHover(bool value);

	void SetSpatialReference(S100_SpatialReference* value);
	void SetSpatialReference(std::list<S100_SpatialReference*> value);
	void SetViewingGroup(const std::string& value);
	void SetViewingGroup(const std::wstring& value);
	void SetViewingGroup(std::vector<std::string>& value);
	void SetViewingGroup(std::vector<std::string_view>& value);
	void SetDisplayPlane(const std::string& value);
	void SetDisplayPlane(const std::wstring& value);
	void SetDrawingPriority(const std::string& value);
	void SetDrawingPriority(const std::wstring& value);
	void SetDrawingPriority(int value);
	void SetScaleMinimum(const std::string& value);
	void SetScaleMinimum(const std::wstring& value);
	void SetScaleMinimum(int value);
	void SetScaleMaximum(const std::string& value);
	void SetScaleMaximum(const std::wstring& value);
	void SetScaleMaximum(int value);
	void setAlertReference(S100_AlertReference value);

	int GetType();
	const std::string& GetFeatureReference();
	std::wstring GetFeatureReferenceW();
	const std::string getId();
	const std::string getParentId();
	const bool getHover();
	S100_SpatialReference* GetSpatialReference(int index);
	std::list<S100_SpatialReference*> GetSpatialReference();
	std::string  GetViewingGroup(int index);
	std::wstring GetViewingGroupW(int index);
	int getViewingGroupCount();
	std::string  GetDisplayPlane();
	std::wstring GetDisplayPlaneW();
	int GetDrawingProiority();
	int GetScaleMinimum();
	int GetScaleMaximum();
	bool hasAlertReference();
	S100_AlertReference getAlertReference();

	// timeValid
	void addTimeValid(S100_TM_Period value);
	std::vector<S100_TM_Period> getTimeValid();
};
