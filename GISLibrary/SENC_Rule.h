#pragma once

#include "SENC_Template.h"
#include "SENC_Attribute.h"

#include <vector>
#include <unordered_map>

class SENC_Rule
{
public:
	SENC_Rule();
	virtual ~SENC_Rule();

public:
	std::vector<SENC_Template*> templates;
	std::vector<std::unordered_map<std::wstring, std::vector<SENC_Template*>*>> searchTablePoint;
	std::vector<std::unordered_map<std::wstring, std::vector<SENC_Template*>*>> searchTableCurve;
	std::vector<std::unordered_map<std::wstring, std::vector<SENC_Template*>*>> searchTableSurface;

public:
	void OrganizeSearchTable();
	SENC_Template* FindPortrayalObject(int typeOfGeometry, std::wstring camelCase, std::vector<SENC_Attribute*> &attrList);
};