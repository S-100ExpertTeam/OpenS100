#pragma once
#include "SENC_LineStyleBase.h"
#include <string>

class S100_LineStyleReference;
class SENC_LineStyleReference : public SENC_LineStyleBase
{
public:
	SENC_LineStyleReference();
	virtual ~SENC_LineStyleReference();

public:
	std::wstring reference;

public:
	void GetStyleFromS100(S100_LineStyleReference* lineStyle);

	void SetReference(const std::wstring& value);
	std::wstring GetReference();

public: // ovveride from SENC_LineStyleBase
	void Accept(SENC_LineStyleVisitor& visitor) override { visitor.Visit(*this); }
};