#pragma once
#include <list>

class PortrayalCatalogue;
class SENC_Instruction;
class DrawingSet
{
public:
	DrawingSet();
	virtual ~DrawingSet();

private:
	std::list<SENC_Instruction*> augmentedRayList[100];
	std::list<SENC_Instruction*> augmentedPathList[100];
	std::list<SENC_Instruction*> pointList[100];
	std::list<SENC_Instruction*> lineList[100];
	std::list<SENC_Instruction*> areaList[100];
	std::list<SENC_Instruction*> textList[100];

public:
	std::list<SENC_Instruction*>* GetAugmentedRayList(int _index);
	std::list<SENC_Instruction*>* GetAugmentedPathList(int _index);
	std::list<SENC_Instruction*>* GetPointList(int _index);
	std::list<SENC_Instruction*>* GetLineList(int _index);
	std::list<SENC_Instruction*>* GetAreaList(int _index);
	std::list<SENC_Instruction*>* GetTextList(int _index);

	std::list<SENC_Instruction*>* GetAugmentedRayList();
	std::list<SENC_Instruction*>* GetAugmentedPathList();
	std::list<SENC_Instruction*>* GetPointList();
	std::list<SENC_Instruction*>* GetLineList();
	std::list<SENC_Instruction*>* GetAreaList();
	std::list<SENC_Instruction*>* GetTextList();
};