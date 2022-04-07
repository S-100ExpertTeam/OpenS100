#pragma once

#include <string>

class S101Cell;

class S100_DisplayList;
class SENC_DisplayList;
class PortrayalCatalogue;
class S100_NullInstruction;
class S100_PointInstruction;
class S100_LineInstruction;
class S100_AreaInstruction;
class S100_TextInstruction;
class S100_AugmentedRay;
class S100_AugmentedPath;
class S100_Instruction;
class SENC_Instruction;
class SENC_NullInstruction;
class SENC_PointInstruction;
class SENC_LineInstruction;
class SENC_AreaInstruction;
class SENC_TextInstruction;
class SENC_AugmentedRay;
class SENC_AugmentedPath;
class PCOutputSchemaManager
{
public:
	PCOutputSchemaManager();
	virtual ~PCOutputSchemaManager();
	
public:
	S100_DisplayList* displayList = nullptr;
	// Lua Drawing Instructions
	SENC_DisplayList* displayListSENC = nullptr;

public:
	void GenerateSENCInstruction(S101Cell* cell, PortrayalCatalogue* pc);
	void InitDisplayList();
	void GenerateSENCAlertInstruction(S101Cell* cell, PortrayalCatalogue* pc);
	SENC_NullInstruction* GetSENCFromS100(S100_NullInstruction*, PortrayalCatalogue* pc);
	SENC_PointInstruction* GetSENCFromS100(S100_PointInstruction*, PortrayalCatalogue* pc);
	SENC_LineInstruction* GetSENCFromS100(S100_LineInstruction*, PortrayalCatalogue* pc);
	SENC_AreaInstruction* GetSENCFromS100(S100_AreaInstruction*, PortrayalCatalogue* pc);
	SENC_TextInstruction* GetSENCFromS100(S100_TextInstruction*, PortrayalCatalogue* pc);
	SENC_AugmentedRay* GetSENCFromS100(S100_AugmentedRay*, PortrayalCatalogue* pc);
	SENC_AugmentedPath* GetSENCFromS100(S100_AugmentedPath*, PortrayalCatalogue* pc);
	void GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si);
	void ChangePallete(PortrayalCatalogue *pc);
};