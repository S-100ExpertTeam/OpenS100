#include "stdafx.h"
#include "LineStyleCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
	LineStyle::LineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency, const std::string& capStyle, const std::string& joinStyle, double offset)
		: name(name), intervalLength(intervalLength), width(width), token(token), transparency(transparency), capStyle(capStyle), joinStyle(joinStyle), offset(offset) {}

	void LineStyle::init()
	{
		Command::init();
		name.clear();
		intervalLength = 0.0;
		width = 0.0;
		token.clear();
		transparency = 0.0;
		capStyle = "Butt";
		joinStyle = "Miter";
		offset = 0.0;
	}

	void LineStyle::execute() 
	{
		// Execute LineStyle
	}

	void LineStyle::parse(const std::string& input)
	{
		setPresent();
		// LineStyle:name,intervalLength,width,token[,transparency[,capStyle[,joinStyle[,offset]]]] 
		std::vector<std::string> values = LatLonUtility::Split(input, ",");
		if (values.size() >= 4)
		{
			name = values[0];
			try
			{
				intervalLength = std::stod(values[1]);
				width = std::stod(values[2]);
				token = values[3];
				
				if (values.size() >= 5)
				{
					transparency = std::stod(values[4]);
				}
				if (values.size() >= 6)
				{
					capStyle = values[5];
				}
				if (values.size() >= 7)
				{
					joinStyle = values[6];
				}
				if (values.size() >= 8)
				{
					offset = std::stod(values[7]);
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

	LineSymbol::LineSymbol(const std::string& Reference, double position, double rotation, const GraphicBasePackage::CRSType crsType)
		: reference(Reference), position(position), rotation(rotation), crsType(crsType) {}

	void LineSymbol::init()
	{
		Command::init();
		reference;
		position = 0.0;
		rotation = 0.0;
		crsType = GraphicBasePackage::CRSType::localCRS;
		scaleFactor = 1.0;
	}

	void LineSymbol::execute() 
	{
		// Execute LineSymbol
	}

	void LineSymbol::parse(const std::string& input)
	{
		setPresent();
		// LineSymbol:reference,position[,rotation[,crsType[,scaleFactor]]] 
		std::vector<std::string> values = LatLonUtility::Split(input, ",");
		if (values.size() >= 2) 
		{
			reference = values[0];
			try
			{
				position = std::stod(values[1]);
				if (values.size() >= 3)
				{
					rotation = std::stod(values[2]);
				}
				if (values.size() >= 4)
				{
					if (values[3].compare("LocalCRS") == 0)
					{
						crsType = GraphicBasePackage::CRSType::localCRS;
					}
					else if (values[3].compare("LineCRS") == 0)
					{
						crsType = GraphicBasePackage::CRSType::lineCRS;
					}
					else if (values[3].compare("PortrayalCRS") == 0)
					{
						crsType = GraphicBasePackage::CRSType::portrayalCRS;
					}
				}
				if (values.size() >= 5)
				{
					scaleFactor = std::stod(values[4]);
				}
			}
			catch (const std::exception& e) 
			{
				init();
			}

			if (values.size() >= 3)
			{

			}
		} else {
			init();
		}
	}

	Dash::Dash(double start, double length)
		: start(start), length(length) {}

	void Dash::init()
	{
		Command::init();
		start = 0.0;
		length = 0.0;
	}

	void Dash::execute() 
	{
		// Execute Dash
	}
	void Dash::parse(const std::string& input)
	{
		setPresent();
		// Dash:start,length 
		std::vector<std::string> values = LatLonUtility::Split(input, ",");
		if (values.size() == 2) {
			try {
				start = std::stod(values[0]);
				length = std::stod(values[1]);
			} catch (const std::exception& e) {
				init();
			}
		} else {
			init();
		}
	}
}