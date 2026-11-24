#pragma once

#include <vector>

class PortrayalCatalogue;
class SENC_Instruction;

// The instructions of one kind and one drawing priority, in the order they are drawn. A
// vector: emptied for the next frame it keeps its storage, where a list would free and
// allocate a node an instruction every frame.
using InstructionList = std::vector<SENC_Instruction*>;

class DrawingSet
{
public:
	DrawingSet();
	virtual ~DrawingSet();

private:
	InstructionList augmentedRayList[100];
	InstructionList augmentedPathList[100];
	InstructionList pointList[100];
	InstructionList lineList[100];
	InstructionList areaList[100];
	InstructionList textList[100];

public:
	InstructionList* GetAugmentedRayList(int _index);
	InstructionList* GetAugmentedPathList(int _index);
	InstructionList* GetPointList(int _index);
	InstructionList* GetLineList(int _index);
	InstructionList* GetAreaList(int _index);
	InstructionList* GetTextList(int _index);

	InstructionList* GetAugmentedRayList();
	InstructionList* GetAugmentedPathList();
	InstructionList* GetPointList();
	InstructionList* GetLineList();
	InstructionList* GetAreaList();
	InstructionList* GetTextList();

public:
	void Init();
};