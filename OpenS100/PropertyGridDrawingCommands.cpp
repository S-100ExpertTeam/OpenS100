#include "pch.h"
#include "PropertyGridDrawingCommands.h"

CPropertyGridDrawingCommands::CPropertyGridDrawingCommands()
{

}

CPropertyGridDrawingCommands::~CPropertyGridDrawingCommands()
{

}

//Receive a drawing information string.
void CPropertyGridDrawingCommands::SetDrawingCommands(std::string value)
{
	this->SetWindowTextW(L""); //Erase all the details.

	std::wstring _value;
	_value.assign(value.begin(), value.end());

	this->SetWindowTextW(_value.c_str());
}