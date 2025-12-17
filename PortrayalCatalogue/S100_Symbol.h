#pragma once

//#include "S100_Rotation.h"
#include "S100_AreaPlacement.h"
#include "S100_LinePlacement.h"
#include "S100_LineSymbolPlacement.h"
#include "S100_AreaSymbolPlacement.h"

#include "..\\S100Engine\\GraphicBasePackage_Enum.h"
#include "..\\S100Engine\\GraphicBasePackage.h"

#include <pugixml.hpp>

#include <optional>
#include <vector>

class S100_Description;

class S100_Symbol
{
public:
	S100_Symbol();
	virtual ~S100_Symbol();

private:
	std::wstring reference;
	double rotation = 0;
	GraphicBasePackage::CRSType rotationCRS = GraphicBasePackage::CRSType::portrayalCRS;
	double scaleFactor = 1.0;
	GraphicBasePackage::Vector offset;

private:
	std::optional<S100_LineSymbolPlacement> linePlacement;
	std::optional<S100_AreaSymbolPlacement> areaPlacement;

private:
	std::optional<GraphicBasePackage::Color> overrideAll;
	std::vector<GraphicBasePackage::Color> override;

	//std::wstring id;
	//S100_Description* description;
	//std::wstring fileName;
	//std::wstring fileType;
	//std::wstring fileFormat;

public:
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	void SetReference(std::wstring& value);
	void SetReference(std::string& value);
	void SetRotation(double value);
	void SetRotationCRS(std::string& value);
	void SetRotationCRS(std::wstring& value);
	void SetScaleFactor(double value);

	std::wstring GetReference();
	double GetRotation();
	//std::wstring GetRotationCRS();
	GraphicBasePackage::CRSType GetRotationCRS();
	double GetScaleFactor();
	std::optional<S100_LineSymbolPlacement> GetLinePlacement();
	std::optional<S100_AreaSymbolPlacement> GetAreaPlacement();

	void SetLinePlacement(double offset, S100_LinePlacementMode placementMode, std::optional<bool> visibleParts);
	void SetAreaPlacement(S100_AreaPlacementMode mode);

	//S100_AreaPlacement* GetAreaPlacement();
	//S100_LinePlacement* GetLinePlacement();
	//std::wstring GetId();

	//S100_Description* GetDescription();
	//std::wstring GetFileName();
	//std::wstring GetFileType();
	//std::wstring GetFileFormat();

	double GetOffsetX();
	void SetOffsetX(double value);

	double GetOffsetY();
	void SetOffsetY(double value);
};