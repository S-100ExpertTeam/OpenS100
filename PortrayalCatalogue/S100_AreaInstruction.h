#pragma once

#include "S100_Instruction.h"
#include "S100_AreaFillBase.h"

class S100_AreaInstruction : public S100_Instruction
{
public:
	S100_AreaInstruction();
	virtual ~S100_AreaInstruction();

private:
	S100_AreaFillBase *areaFill;

public:
	void GetContents(pugi::xml_node node);

	void SetAreaFill(S100_AreaFillBase* value);
	void SetAreaFill(std::wstring& token, std::wstring& name, std::wstring& transparency);
	void SetAreaFill(std::wstring& reference);
	S100_AreaFillBase* GetAreaFill();
};