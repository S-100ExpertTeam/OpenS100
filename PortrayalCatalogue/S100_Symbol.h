#pragma once
#include "S100_Rotation.h"
#include "S100_AreaPlacement.h"
#include "S100_LinePlacement.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

class S100_Description;
class S100_Symbol
{
public:
	S100_Symbol();
	virtual ~S100_Symbol();

private:
	std::wstring reference;
	S100_Rotation *rotation;
	std::wstring rotationCRS;
	std::wstring scaleFactor;

	S100_AreaPlacement *areaPlacement;
	S100_LinePlacement *linePlacement;

	//pc º¯¼ö 
	S100_Description* description;
	std::wstring fileName;
	std::wstring fileType;
	std::wstring fileFormat;


public:
	void GetContents(pugi::xml_node& node);

	void SetReference(std::wstring& value);
	void SetRotation(S100_Rotation* value);
	void SetRotationCRS(std::wstring& value);
	void SetScaleFactor(std::wstring& value);
	void SetAreaPlacement(S100_AreaPlacement* value);
	void SetLinePlacement(S100_LinePlacement* value);

	std::wstring GetReference();
	S100_Rotation* GetRotation();
	std::wstring GetRotationCRS();
	std::wstring GetScaleFactor();
	S100_AreaPlacement* GetAreaPlacement();
	S100_LinePlacement* GetLinePlacement();
};