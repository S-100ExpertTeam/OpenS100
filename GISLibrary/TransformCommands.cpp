#include "stdafx.h"
#include "TransformCommands.h"
#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingCommand
{
	// LocalOffset class implementation
	LocalOffset::LocalOffset(double xOffsetMM, double yOffsetMM) : xOffsetMM(xOffsetMM), yOffsetMM(yOffsetMM) {}

	Enum_StateCommand LocalOffset::GetType() const
	{
		return Enum_StateCommand::LocalOffset;
	}

	void LocalOffset::init()
	{
		Command::init();
		xOffsetMM = 0.0;
		yOffsetMM = 0.0;
	}

	void LocalOffset::execute() 
	{
	}

	void LocalOffset::parse(const std::string& input)
	{
		setPresent();
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

	double LocalOffset::GetXOffsetMM() const
	{
		return xOffsetMM;
	}

	double LocalOffset::GetYOffsetMM() const
	{
		return yOffsetMM;
	}

	// LinePlacement class implementation
	LinePlacement::LinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts)
		: linePlacementMode(linePlacementMode), offset(offset), endOffset(endOffset), visibleParts(visibleParts) {}

	Enum_StateCommand LinePlacement::GetType() const
	{
		return Enum_StateCommand::LinePlacement;
	}

	void LinePlacement::init()
	{
		Command::init();
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
		setPresent();
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

	std::string LinePlacement::GetLinePlacementMode() const
	{
		return linePlacementMode;
	}

	double LinePlacement::GetOffset() const
	{
		return offset;
	}

	std::optional<double> LinePlacement::GetEndOffset() const
	{
		return endOffset;
	}

	bool LinePlacement::IsVisibleParts() const
	{
		return visibleParts;
	}

	// AreaPlacement class implementation
	AreaPlacement::AreaPlacement(const std::string& areaPlacementMode) : areaPlacementMode(areaPlacementMode) {}

	Enum_StateCommand AreaPlacement::GetType() const
	{
		return Enum_StateCommand::AreaPlacement;
	}

	void AreaPlacement::init()
	{
		Command::init();
		areaPlacementMode = "VisibleParts";
	}

	void AreaPlacement::execute() 
	{
	}

	void AreaPlacement::parse(const std::string& input)
	{
		setPresent();
		// AreaPlacement:areaPlacementMode
		areaPlacementMode = input;
	}

	std::string AreaPlacement::GetAreaPlacementMode() const
	{
		return areaPlacementMode;
	}

	// AreaCRS class implementation
	AreaCRS::AreaCRS(const std::string& areaCRSType) : areaCRSType(areaCRSType) {}

	Enum_StateCommand AreaCRS::GetType() const
	{
		return Enum_StateCommand::AreaCRS;
	}

	void AreaCRS::init()
	{
		Command::init();
		areaCRSType = "ViGlobalGeometrysibleParts";
	}

	void AreaCRS::execute() {
	}

	void AreaCRS::parse(const std::string& input)
	{
		setPresent();
		// AreaCRS:areaCRSType
		areaCRSType = input;
	}

	// Rotation class implementation
	Rotation::Rotation(const std::string& rotationCRS, double rotation) : rotationCRS(rotationCRS), rotation(rotation) {}

	Enum_StateCommand Rotation::GetType() const
	{
		return Enum_StateCommand::Rotation;
	}

	void Rotation::init()
	{
		Command::init();
		rotationCRS = "PortrayalCRS";
		rotation = 0.0;
	}

	void Rotation::execute() 
	{
	}

	void Rotation::parse(const std::string& input)
	{
		setPresent();
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

	std::string Rotation::GetRotationCRS() const
	{
		return rotationCRS;
	}

	double Rotation::GetRotation() const
	{
		return rotation;
	}

	// ScaleFactor class implementation
	ScaleFactor::ScaleFactor(double scaleFactor) : scaleFactor(scaleFactor) {}

	Enum_StateCommand ScaleFactor::GetType() const
	{
		return Enum_StateCommand::ScaleFactor;
	}

	void ScaleFactor::init()
	{
		Command::init();
		scaleFactor = 1.0;
	}

	void ScaleFactor::execute() 
	{
	}

	void ScaleFactor::parse(const std::string& input)
	{
		setPresent();
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

	double ScaleFactor::GetScaleFactor() const
	{
		return scaleFactor;
	}
}