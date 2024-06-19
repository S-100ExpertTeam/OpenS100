#include "stdafx.h"
#include "ColourOverrideCommands.h"

namespace DrawingInstruction
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

	void ColourOverrideCommands::executeCommands() const
	{
		if (overrideColor) overrideColor->execute();
		if (overrideAll) overrideAll->execute();
	}


	OverrideColor::OverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency)
		: colorToken(colorToken), colorTransparency(colorTransparency), overrideToken(overrideToken), overrideTransparency(overrideTransparency) {}

	void OverrideColor::execute()
	{
	}


	void OverrideColor::parse(const std::string& input)
	{
	}

	OverrideAll::OverrideAll(const std::string& token, double transparency)
		: token(token), transparency(transparency) {}
	void OverrideAll::execute()  {
	}
	void OverrideAll::parse(const std::string& input)
	{
	}
}
