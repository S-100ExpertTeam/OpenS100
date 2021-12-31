#include "stdafx.h"
#include "DrawingSet.h"

DrawingSet::DrawingSet()
{

}

DrawingSet::~DrawingSet() 
{

}

std::list<SENC_Instruction*>* DrawingSet::GetAugmentedRayList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &augmentedRayList[_index]; 
}

std::list<SENC_Instruction*>* DrawingSet::GetAugmentedPathList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
		
	return &augmentedPathList[_index];
}

std::list<SENC_Instruction*>* DrawingSet::GetPointList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &pointList[_index];
}

std::list<SENC_Instruction*>* DrawingSet::GetLineList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &lineList[_index];
};

std::list<SENC_Instruction*>* DrawingSet::GetAreaList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &areaList[_index];
};

std::list<SENC_Instruction*>* DrawingSet::GetTextList(int _index) 
{ 
	if (_index >= 100)
	{
		return nullptr;
	}
	
	return &textList[_index];
};

std::list<SENC_Instruction*>* DrawingSet::GetAugmentedRayList() 
{ 
	return &augmentedRayList[0]; 
};

std::list<SENC_Instruction*>* DrawingSet::GetAugmentedPathList() 
{ 
	return &augmentedPathList[0]; 
};

std::list<SENC_Instruction*>* DrawingSet::GetPointList() 
{ 
	return &pointList[0]; 
};

std::list<SENC_Instruction*>* DrawingSet::GetLineList() 
{
	return &lineList[0]; 
};

std::list<SENC_Instruction*>* DrawingSet::GetAreaList() 
{
	return &areaList[0]; 
};

std::list<SENC_Instruction*>* DrawingSet::GetTextList()
{ 
	return &textList[0]; 
};