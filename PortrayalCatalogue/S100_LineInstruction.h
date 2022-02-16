#pragma once
#include "S100_Instruction.h"
#include "S100_LineStyleReference.h"
#include "S100_CompositeLineStyle.h"

#include  <vector>

class S100_LineInstruction : public S100_Instruction
{
public:
	S100_LineInstruction();
	virtual ~S100_LineInstruction();

private:
	S100_LineStyleReference* lineStyleReference;
	S100_CompositeLineStyle* compositeLineStyle;
	S100_LineStyle *lineStyle;

private:
	std::wstring suppression = L"false";

public:
	void SetLineStyleReference(S100_LineStyleReference* value);
	S100_LineStyleReference* GetLineStyleReference();

	void SetCompositeLineStyle(S100_CompositeLineStyle* value);
	S100_CompositeLineStyle* GetCompositeLineStyle();

	void SetLineStyle(S100_LineStyle* value);
	S100_LineStyle* GetLineStyle();

	void SetSuppression(std::wstring& value);
	std::wstring GetSuppression();

	void GetContents(pugi::xml_node node);
	bool SuppressionIsTrue();
};