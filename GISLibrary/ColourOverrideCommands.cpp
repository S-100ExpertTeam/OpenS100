#include "stdafx.h"
#include "ColourOverrideCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
	ColourOverrideCommands::~ColourOverrideCommands()
	{
		delete overrideColor;
		overrideColor = nullptr;

		delete overrideAll;
		overrideAll = nullptr;
	
	}
	void ColourOverrideCommands::setOverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency)
	{
		delete this->overrideColor;
		this->overrideColor = new OverrideColor(colorToken, colorTransparency, overrideToken, overrideTransparency);
	}
	void ColourOverrideCommands::setOverrideAll(const std::string& token, double transparency)
	{
		delete this->overrideAll;
		this->overrideAll = new OverrideAll(token, transparency);
	}
	void ColourOverrideCommands::clearOverride()
	{
		delete this->overrideColor;
		this->overrideColor = nullptr;
		delete this->overrideAll;
		this->overrideAll = nullptr;
	}

	void ColourOverrideCommands::parse(const std::string& key, std::string value)
	{
		if (key == "OverrideColor")
		{
			overrideColor->execute();
		}
		else if (key == "OverrideAll")
		{
			overrideAll->execute();
		}
	}

	void ColourOverrideCommands::execute() const
	{
		if (overrideColor) overrideColor->execute();
		if (overrideAll) overrideAll->execute();
	}


	OverrideColor::OverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency)
		: colorToken(colorToken), colorTransparency(colorTransparency), overrideToken(overrideToken), overrideTransparency(overrideTransparency) {}

	void OverrideColor::init()
	{
		StateCommand::init();
		colorToken.clear();
		colorTransparency = 0.0;
		overrideToken.clear();
		overrideTransparency = 0.0;
	}

	void OverrideColor::execute()
	{
	}


	void OverrideColor::parse(const std::string& input)
	{
		// OverrideColor:colorToken,colorTransparency,overrideToken,overrideTransparency 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 4)
		{
			colorToken = tokens[0];
			colorTransparency = std::stod(tokens[1]);
			overrideToken = tokens[2];
			overrideTransparency = std::stod(tokens[3]);
		}
		else
		{
			init(); // Reset to default if parsing fails
		}
	}

	OverrideAll::OverrideAll(const std::string& token, double transparency)
		: token(token), transparency(transparency) {}

	void OverrideAll::init()
	{
		StateCommand::init();
		token.clear();
		transparency = 0.0;
	}

	void OverrideAll::execute()  {
	}
	void OverrideAll::parse(const std::string& input)
	{
		// OverrideAll:token,transparency 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 2)
		{
			token = tokens[0];
			transparency = std::stod(tokens[1]);
		}
		else
		{
			init(); // Reset to default if parsing fails
		}
	}

	void ClearOverride::init()
	{
		StateCommand::init();
	}

	void ClearOverride::execute()  
	{
	}

	void ClearOverride::parse(const std::string& input)
	{
		// ClearOverride : No parameters expected
		// No specific parsing needed for clear override
	}
}
