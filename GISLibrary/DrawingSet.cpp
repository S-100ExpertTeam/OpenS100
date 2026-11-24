#include "stdafx.h"
#include "DrawingSet.h"

DrawingSet::DrawingSet()
{

}

DrawingSet::~DrawingSet() 
{

}

InstructionList* DrawingSet::GetAugmentedRayList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &augmentedRayList[_index]; 
}

InstructionList* DrawingSet::GetAugmentedPathList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
		
	return &augmentedPathList[_index];
}

InstructionList* DrawingSet::GetPointList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &pointList[_index];
}

InstructionList* DrawingSet::GetLineList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &lineList[_index];
};

InstructionList* DrawingSet::GetAreaList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &areaList[_index];
};

InstructionList* DrawingSet::GetTextList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &textList[_index];
};

InstructionList* DrawingSet::GetAugmentedRayList() 
{ 
	return &augmentedRayList[0]; 
};

InstructionList* DrawingSet::GetAugmentedPathList() 
{ 
	return &augmentedPathList[0]; 
};

InstructionList* DrawingSet::GetPointList() 
{ 
	return &pointList[0]; 
};

InstructionList* DrawingSet::GetLineList() 
{
	return &lineList[0]; 
};

InstructionList* DrawingSet::GetAreaList() 
{
	return &areaList[0]; 
};

InstructionList* DrawingSet::GetTextList()
{ 
	return &textList[0]; 
};

void DrawingSet::Init()
{
	for (int i = 0; i < 100; i++)
	{
		augmentedRayList[i].clear();
		augmentedPathList[i].clear();
		pointList[i].clear();
		lineList[i].clear();
		areaList[i].clear();
		textList[i].clear();
	}
}