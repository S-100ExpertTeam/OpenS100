#pragma once

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
	SENC_DisplayList* displayListSENC = nullptr;

public:
	void GenerateSENCInstruction(S100SpatialObject* s100so, PortrayalCatalogue* pc);
	void ParsingSENCInstruction(S100Layer* layer, std::string featureID, std::vector<std::string> elements);
	void InitDisplayList();
	void GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si);
	void GetSENCFromString(SENC_Instruction* sit, std::string fid, std::string dpt, std::string dp, std::string vg, std::string min, std::string max, std::list<std::string> sr);
	void ChangePallete(PortrayalCatalogue *pc);
};