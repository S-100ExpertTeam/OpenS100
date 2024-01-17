#pragma once

#include <list>
#include <map>
#include <set>

class SENC_Instruction;
class SENC_AlertReference;
class Scaler;
class FeatureCatalogue;

class SENC_DisplayList
{
public:
	SENC_DisplayList();
	virtual ~SENC_DisplayList();

public:
	// key : priority + type
	std::multimap<__int64, SENC_Instruction*> displayInstructions;

	// key : id
	std::unordered_map<std::string, SENC_AlertReference*> alertInstructions;

	std::set<std::wstring> offFeatureCode;

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
	void GetDrawingInstruction(int priority, int type, int currentScale, Scaler* scaler, std::list<SENC_Instruction*>& itList);

	void ChangePallete(PortrayalCatalogue *pc);

	void AddInstruction(SENC_Instruction* value);
	void AddAlertIndication(SENC_Instruction* value);

	int GetType(int type);
	__int64 GetKey(int priority, int type);
};