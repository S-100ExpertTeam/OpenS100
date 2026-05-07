#include "stdafx.h"
#include "ColorFill.h"

#include "..\\LatLonUtility\\LatLonUtility.h"
#include "..\\LatLonUtility\\cpp_util.h"

namespace Part9a
{
	ColorFill::ColorFill(const std::string& token, double transparency) : token(token), transparency(transparency)
	{
	}

	void ColorFill::init()
	{
		Command::init();
		token.clear();
		transparency = 0.0;
	}

	void ColorFill::execute()
	{
	}

	void ColorFill::parse(const std::string& input)
	{
		setPresent();
		// ColorFill:token[,transparency] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() > 0)
		{
			token = tokens[0];
		}

		if (tokens.size() > 1)
		{
			try
			{
				transparency = cpp_util::stod(tokens[1]);
			}
			catch (...)
			{
				transparency = 0.0; // Default value in case of error
			}
		}
	}
}
