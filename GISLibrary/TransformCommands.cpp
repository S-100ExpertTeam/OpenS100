#include "stdafx.h"
#include "TransformCommands.h"
#include "..\\LatLonUtility\\LatLonUtility.h"

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

	void LocalOffset::init()
	{
		StateCommand::init();
		xOffsetMM = 0.0;
		yOffsetMM = 0.0;
	}

	void LocalOffset::execute() 
	{
	}

	void LocalOffset::parse(const std::string& input)
	{
		// LocalOffset:xOffsetMM, yOffsetMM

		std::vector<std::string> parts = LatLonUtility::Split(input, ",");
		if (parts.size() == 2) 
		{
			try 
			{
				xOffsetMM = std::stod(parts[0]);
				yOffsetMM = std::stod(parts[1]);
			} 
			catch (const std::exception& e) 
			{
				init();
			}
		} 
		else 
		{
			init();
		}
	}

	// LinePlacement class implementation
	LinePlacement::LinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts)
		: linePlacementMode(linePlacementMode), offset(offset), endOffset(endOffset), visibleParts(visibleParts) {}

	void LinePlacement::init()
	{
		StateCommand::init();
		linePlacementMode = "Relative";
		offset = 0.5;
		endOffset.reset();
		visibleParts = false;
	}

	void LinePlacement::execute() 
	{
	}

	void LinePlacement::parse(const std::string& input)
	{
		// LinePlacement:linePlacementMode, offset[, endOffset][, visibleParts]

		std::vector<std::string> parts = LatLonUtility::Split(input, ",");
		if (parts.size() >= 2) 
		{
			linePlacementMode = parts[0];
			try 
			{
				offset = std::stod(parts[1]);
				if (parts.size() > 2) 
				{
					endOffset = std::stod(parts[2]);
				}
				if (parts.size() > 3) 
				{
					if (parts[3] == "true")
					{
						visibleParts = true;
					}
					else if (parts[3] == "false")
					{
						visibleParts = false;
					}
				}
			} 
			catch (const std::exception& e) 
			{
				init();
			}
		} 
		else 
		{
			init();
		}
	}

	// AreaPlacement class implementation
	AreaPlacement::AreaPlacement(const std::string& areaPlacementMode) : areaPlacementMode(areaPlacementMode) {}

	void AreaPlacement::init()
	{
		StateCommand::init();
		areaPlacementMode = "VisibleParts";
	}

	void AreaPlacement::execute() 
	{
	}

	void AreaPlacement::parse(const std::string& input)
	{
		// AreaPlacement:areaPlacementMode
		areaPlacementMode = input;
	}

	// AreaCRS class implementation
	AreaCRS::AreaCRS(const std::string& areaCRSType) : areaCRSType(areaCRSType) {}

	void AreaCRS::init()
	{
		StateCommand::init();
		areaCRSType = "ViGlobalGeometrysibleParts";
	}

	void AreaCRS::execute() {
	}

	void AreaCRS::parse(const std::string& input)
	{
		// AreaCRS:areaCRSType
		areaCRSType = input;
	}

	// Rotation class implementation
	Rotation::Rotation(const std::string& rotationCRS, double rotation) : rotationCRS(rotationCRS), rotation(rotation) {}

	void Rotation::init()
	{
		StateCommand::init();
		rotationCRS = "PortrayalCRS";
		rotation = 0.0;
	}

	void Rotation::execute() 
	{
	}

	void Rotation::parse(const std::string& input)
	{
		// Rotation:rotationCRS,rotation 
		std::vector<std::string> parts = LatLonUtility::Split(input, ",");
		if (parts.size() == 2) 
		{
			rotationCRS = parts[0];
			try 
			{
				rotation = std::stod(parts[1]);
			} 
			catch (const std::exception& e) 
			{
				init();
			}
		} 
		else 
		{
			init();
		}
	}

	// ScaleFactor class implementation
	ScaleFactor::ScaleFactor(double scaleFactor) : scaleFactor(scaleFactor) {}

	void ScaleFactor::init()
	{
		StateCommand::init();
		scaleFactor = 1.0;
	}

	void ScaleFactor::execute() 
	{
	}

	void ScaleFactor::parse(const std::string& input)
	{
		// ScaleFactor:scaleFactor 
		try 
		{
			scaleFactor = std::stod(input);
		} 
		catch (const std::exception& e) 
		{
			init();
		}
	}
}