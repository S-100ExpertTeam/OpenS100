#pragma once
#include <afxpropertygridctrl.h>
#include <string>

class CPropertyGridDrawingCommands :
	public CEdit
{
public:
	CPropertyGridDrawingCommands();
	virtual ~CPropertyGridDrawingCommands();

public:
	void SetDrawingCommands(std::string value);
};