#include "stdafx.h"
#include "S100_Instruction.h"

S100_Instruction::S100_Instruction() : type(0)
{

}

S100_Instruction::~S100_Instruction()
{
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		delete *itor;
	}
}

void S100_Instruction::GetContents(pugi::xml_node node)
{

}

void S100_Instruction::SetType(int value)
{
	type = value;
}

void S100_Instruction::SetFeatureReference(const std::string& value)  { featureReference = value; }
void S100_Instruction::SetFeatureReference(const std::wstring& value) { featureReference = toUtf8(value); }
void S100_Instruction::SetFeatureReference(std::string_view value)    { featureReference = std::string(value); }

void S100_Instruction::setId(std::string value)       { id = value; }
void S100_Instruction::setParentId(std::string value) { parentId = value; }
void S100_Instruction::setHover(bool value)           { hover = value; }

void S100_Instruction::SetSpatialReference(S100_SpatialReference* value)
{
	spatialReference.push_back(value);
}

void S100_Instruction::SetSpatialReference(std::list<S100_SpatialReference*> value)
{
	spatialReference = value;
}

void S100_Instruction::SetViewingGroup(const std::string& value)
{
	viewingGroup.push_back(value);
}

void S100_Instruction::SetViewingGroup(const std::wstring& value)
{
	viewingGroup.push_back(toUtf8(value));
}

void S100_Instruction::SetViewingGroup(std::vector<std::string>& value)
{
	for (const auto& v : value)
	{
		viewingGroup.push_back(v);
	}
}

void S100_Instruction::SetViewingGroup(std::vector<std::string_view>& value)
{
	for (const auto& v : value)
	{
		viewingGroup.push_back(std::string(v));
	}
}

void S100_Instruction::SetDisplayPlane(const std::string& value)  { displayPlane = value; }
void S100_Instruction::SetDisplayPlane(const std::wstring& value) { displayPlane = toUtf8(value); }

void S100_Instruction::SetDrawingPriority(const std::string& value)
{
	try   { drawingPriority = std::stoi(value); }
	catch (...) { drawingPriority = 0; }
}

void S100_Instruction::SetDrawingPriority(const std::wstring& value)
{
	SetDrawingPriority(toUtf8(value));
}

void S100_Instruction::SetDrawingPriority(int value)
{
	drawingPriority = value;
}

void S100_Instruction::SetScaleMinimum(const std::string& value)
{
	try   { scaleMinimum = std::stoi(value); }
	catch (...) { scaleMinimum = 0; }
}

void S100_Instruction::SetScaleMinimum(const std::wstring& value)
{
	SetScaleMinimum(toUtf8(value));
}

void S100_Instruction::SetScaleMinimum(int value)
{
	scaleMinimum = value;
}

void S100_Instruction::SetScaleMaximum(const std::string& value)
{
	try   { scaleMaximum = std::stoi(value); }
	catch (...) { scaleMaximum = 0; }
}

void S100_Instruction::SetScaleMaximum(const std::wstring& value)
{
	SetScaleMaximum(toUtf8(value));
}

void S100_Instruction::SetScaleMaximum(int value)
{
	scaleMaximum = value;
}

void S100_Instruction::setAlertReference(S100_AlertReference value)
{
	alertReference = value;
}

int S100_Instruction::GetType() { return type; }

const std::string& S100_Instruction::GetFeatureReference() { return featureReference; }
std::wstring S100_Instruction::GetFeatureReferenceW()      { return toWide(featureReference); }

const std::string S100_Instruction::getId()
{
	return id.has_value() ? id.value() : std::string("");
}

const std::string S100_Instruction::getParentId()
{
	return parentId.has_value() ? parentId.value() : std::string("");
}

const bool S100_Instruction::getHover()
{
	return hover.has_value() ? hover.value() : false;
}

S100_SpatialReference* S100_Instruction::GetSpatialReference(int index)
{
	auto it = spatialReference.begin();
	advance(it, index);
	return *it;
}

std::list<S100_SpatialReference*> S100_Instruction::GetSpatialReference()
{
	return spatialReference;
}

std::string S100_Instruction::GetViewingGroup(int index)
{
	if (index >= 0 && index < (int)viewingGroup.size())
	{
		return viewingGroup[index];
	}
	return "";
}

std::wstring S100_Instruction::GetViewingGroupW(int index)
{
	return toWide(GetViewingGroup(index));
}

int S100_Instruction::getViewingGroupCount()
{
	return (int)viewingGroup.size();
}

std::string  S100_Instruction::GetDisplayPlane()  { return displayPlane; }
std::wstring S100_Instruction::GetDisplayPlaneW() { return toWide(displayPlane); }

int S100_Instruction::GetDrawingProiority() { return drawingPriority; }

int S100_Instruction::GetScaleMinimum()
{
	return scaleMinimum.has_value() ? scaleMinimum.value() : 0;
}

int S100_Instruction::GetScaleMaximum()
{
	return scaleMaximum.has_value() ? scaleMaximum.value() : 0;
}

bool S100_Instruction::hasAlertReference()
{
	return alertReference.has_value();
}

S100_AlertReference S100_Instruction::getAlertReference()
{
	return alertReference.has_value() ? alertReference.value() : S100_AlertReference();
}

void S100_Instruction::addTimeValid(S100_TM_Period value)
{
	timeValid.push_back(value);
}

std::vector<S100_TM_Period> S100_Instruction::getTimeValid()
{
	return timeValid;
}
