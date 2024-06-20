#include "stdafx.h"
#include "TransformCommands.h"




namespace DrawingInstructions
{
	TransformCommands::~TransformCommands() {
		delete localOffset;
		delete linePlacement;
		delete areaPlacement;
		delete areaCRS;
		delete rotation;
		delete scaleFactor;
	}

	void TransformCommands::setLocalOffset(double xOffsetMM, double yOffsetMM) {
		delete this->localOffset;
		localOffset = new LocalOffset(xOffsetMM, yOffsetMM);
	}

	void TransformCommands::setLinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts) {
		delete this->linePlacement;
		this->linePlacement = new LinePlacement(linePlacementMode, offset, endOffset, visibleParts);
	}

	void TransformCommands::setAreaPlacement(const std::string& areaPlacementMode) {
		delete this->areaPlacement;
		this->areaPlacement = new AreaPlacement(areaPlacementMode);
	}

	void TransformCommands::setAreaCRS(const std::string& areaCRSType) {
		delete this->areaCRS;
		this->areaCRS = new AreaCRS(areaCRSType);
	}

	void TransformCommands::setRotation(const std::string& rotationCRS, double rotation) {
		delete this->rotation;
		this->rotation = new Rotation(rotationCRS, rotation);
	}

	void TransformCommands::setScaleFactor(double scaleFactor) {
		delete this->scaleFactor;
		this->scaleFactor = new ScaleFactor(scaleFactor);
	}

	void TransformCommands::execute() const
	{
		if (localOffset) localOffset->execute();
		if (linePlacement) linePlacement->execute();
		if (areaPlacement) areaPlacement->execute();
		if (areaCRS) areaCRS->execute();
		if (rotation) rotation->execute();
		if (scaleFactor) scaleFactor->execute();
	}

	void TransformCommands::parse(const std::string& key, std::string value)
	{
		if (key == "LocalOffset")
		{
			//setLocalOffset();
		}
		else if (key == "LinePlacement")
		{
			//setLinePlacement();
		}
		else if (key == "AreaPlacement")
		{
			//setAreaPlacement();
		}
		else if (key == "AreaCRS")
		{
			//setAreaCRS();
		}
		else if (key == "Rotation")
		{
			//setRotation();
		}
		else if (key == "ScaleFactor")
		{
			//setScaleFactor();
		}
	}



	// LocalOffset class implementation
	LocalOffset::LocalOffset(double xOffsetMM, double yOffsetMM) : xOffsetMM(xOffsetMM), yOffsetMM(yOffsetMM) {}

	void LocalOffset::execute() {
	}

	void LocalOffset::parse(const std::string& input)
	{
	}

	// LinePlacement class implementation
	LinePlacement::LinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts)
		: linePlacementMode(linePlacementMode), offset(offset), endOffset(endOffset), visibleParts(visibleParts) {}

	void LinePlacement::execute() {
	}

	void LinePlacement::parse(const std::string& input)
	{
	}

	// AreaPlacement class implementation
	AreaPlacement::AreaPlacement(const std::string& areaPlacementMode) : areaPlacementMode(areaPlacementMode) {}

	void AreaPlacement::execute() {
	}

	void AreaPlacement::parse(const std::string& input)
	{
	}

	// AreaCRS class implementation
	AreaCRS::AreaCRS(const std::string& areaCRSType) : areaCRSType(areaCRSType) {}

	void AreaCRS::execute() {
	}

	void AreaCRS::parse(const std::string& input)
	{
	}

	// Rotation class implementation
	Rotation::Rotation(const std::string& rotationCRS, double rotation) : rotationCRS(rotationCRS), rotation(rotation) {}

	void Rotation::execute() {
	}

	void Rotation::parse(const std::string& input)
	{
	}

	// ScaleFactor class implementation
	ScaleFactor::ScaleFactor(double scaleFactor) : scaleFactor(scaleFactor) {}

	void ScaleFactor::execute() {
	}

	void ScaleFactor::parse(const std::string& input)
	{
	}

}