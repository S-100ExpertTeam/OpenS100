#include "stdafx.h"
#include "ColourOverrideCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
	OverrideColor::OverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency)
		: colorToken(colorToken), colorTransparency(colorTransparency), overrideToken(overrideToken), overrideTransparency(overrideTransparency) {}

	void OverrideColor::init()
	{
		Command::init();
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
		setPresent();
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
		Command::init();
		token.clear();
		transparency = 0.0;
	}

	void OverrideAll::execute()  {
	}
	void OverrideAll::parse(const std::string& input)
	{
		setPresent();
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
		Command::init();
	}

	void ClearOverride::execute()  
	{
	}

	void ClearOverride::parse(const std::string& input)
	{
		setPresent();
		// ClearOverride : No parameters expected
		// No specific parsing needed for clear override
	}
}
