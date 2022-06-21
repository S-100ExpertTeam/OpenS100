#pragma once

#include <list>
#include <map>

class SENC_Instruction;
class SENC_AlertReference;
class Scaler;
class SENC_DisplayList
{
public:
	SENC_DisplayList();
	virtual ~SENC_DisplayList();

public:
	std::multimap<__int64, SENC_Instruction*> displayInstructions;
	std::unordered_map<__int64, SENC_AlertReference*> alertInstructions;

public:
	/*
	* Arg 02 discription
	*  Type Of Instruction
	*  0 : Null Instruction
	*  1 : Point Instruction
	*  2 : Line Instruction
	*  3 : Area Instruction
	** 4 : Coverage Instruction
	*  5 : Text Instruction
	** 6 : Augmented Point
	** 7 : Augmented Ray
	** 8 : Augmented Path
	** 9 : Augmented Area
	*/
	void GetDrawingInstruction(int priority, int type, Scaler* scaler, std::list<SENC_Instruction*>& itList);

	void GetDrawingInstructionByCondition(int priority, int type, Scaler* scaler, std::list<SENC_Instruction*> &itList, int FeatureID_textPlacement);

	void ChangePallete(PortrayalCatalogue *pc);

	void AddInstruction(SENC_Instruction* value);
	void AddAlertIndication(SENC_Instruction* value);

	int GetType(int type);
	__int64 GetKey(int priority, int type);
};