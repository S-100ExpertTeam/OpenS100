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

void S100_Instruction::SetFeatureReference(S100_FeatureReference* value)
{
	featureReference = *value;
}

void S100_Instruction::SetSpatialReference(S100_SpatialReference* value)
{
	spatialReference.push_back(value);
}

void S100_Instruction::SetSpatialReference(std::list<S100_SpatialReference*> value) 
{
	spatialReference = value;
}

void S100_Instruction::SetViewingGroup(std::wstring& value)
{
	viewingGroup = value;
}

void S100_Instruction::SetDisplayPlane(std::wstring& value)
{
	displayPlane = value;
}

void S100_Instruction::SetDrawingProiority(std::wstring& value)
{
	drawingPriority = value;
}
void S100_Instruction::SetScaleMinimum(std::wstring& value)
{
	scaleMinimum = value;
}
void S100_Instruction::SetScaleMaximum(std::wstring& value)
{
	scaleMaximum = value;
}

int S100_Instruction::GetType() 
{
	return type;
}
S100_FeatureReference* S100_Instruction::GetFeatureReference()
{
	return &featureReference;
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
std::wstring S100_Instruction::GetViewingGroup() 
{
	return viewingGroup;
}
std::wstring S100_Instruction::GetDisplayPlane()
{
	return displayPlane;
}
std::wstring S100_Instruction::GetDrawingProiority()
{
	return drawingPriority;
}
std::wstring S100_Instruction::GetScaleMinimum()
{
	return scaleMinimum;
}
std::wstring S100_Instruction::GetScaleMaximum() 
{
	return scaleMaximum;
}