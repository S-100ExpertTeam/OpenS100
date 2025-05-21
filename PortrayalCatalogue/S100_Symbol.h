#pragma once

//#include "S100_Rotation.h"
#include "S100_AreaPlacement.h"
#include "S100_LinePlacement.h"

#include <pugixml.hpp>
class S100_Description;

class S100_Symbol
{
public:
	S100_Symbol();
	virtual ~S100_Symbol();

private:
	std::wstring reference;
	double rotation = 0;
	std::wstring rotationCRS;
	double scaleFactor = 1.0;

	double offsetX = 0;
	double offsetY = 0;

private:
	S100_AreaPlacement* areaPlacement = nullptr;
	S100_LinePlacement* linePlacement = nullptr;


	//std::wstring id;
	//S100_Description* description;
	//std::wstring fileName;
	//std::wstring fileType;
	//std::wstring fileFormat;

public:
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	void SetReference(std::wstring& value);
	void SetRotation(double value);
	void SetRotationCRS(std::wstring& value);
	void SetScaleFactor(double value);

	std::wstring GetReference();
	double GetRotation();
	std::wstring GetRotationCRS();
	double GetScaleFactor();
	S100_AreaPlacement* GetAreaPlacement();
	S100_LinePlacement* GetLinePlacement();
	//std::wstring GetId();

	//S100_Description* GetDescription();
	//std::wstring GetFileName();
	//std::wstring GetFileType();
	//std::wstring GetFileFormat();

	double GetOffsetX();
	double GetOffsetY();
};