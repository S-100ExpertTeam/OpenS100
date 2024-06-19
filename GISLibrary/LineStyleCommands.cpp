#include "stdafx.h"
#include "LineStyleCommands.h"

namespace DrawingInstructions
{
	LineStyleCommands::~LineStyleCommands()
	{
		delete lineStyle;
		lineStyle = nullptr;

		delete lineSymbol;
		lineSymbol = nullptr;

		delete dash;
		dash = nullptr;
	}
	void LineStyleCommands::setLineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency, const std::string& capStyle, const std::string& joinStyle, double offset)
	{
		delete this->lineStyle;
		this->lineStyle = new LineStyle(name, intervalLength, width, token, transparency, capStyle, joinStyle, offset);
	}

	void LineStyleCommands::setLineSymbol(const std::string& Reference, double position, double rotation, const std::string& crsType)
	{
		delete this->lineSymbol;
		this->lineSymbol = new LineSymbol(Reference, position, rotation, crsType);
	}

	void LineStyleCommands::setDash(double start, double length) {
		delete this->dash;
		this->dash = new Dash(start, length);
	}

	void LineStyleCommands::parse(const std::string& key, std::string value)
	{
		if (key == "LineStyle")
		{
			lineStyle->execute();
		}
		else if (key == "LineSymbol")
		{
			lineSymbol->execute();
		}
		else if (key == "Dash")
		{
			dash->execute();
		}
	}

	void LineStyleCommands::execute() const
	{
		if (lineStyle) lineStyle->execute();
		if (lineSymbol) lineSymbol->execute();
		if (dash) dash->execute();
	}

	LineStyle::LineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency, const std::string& capStyle, const std::string& joinStyle, double offset)
		: name(name), intervalLength(intervalLength), width(width), token(token), transparency(transparency), capStyle(capStyle), joinStyle(joinStyle), offset(offset) {}

	void LineStyle::execute() 
	{
		// Execute LineStyle
	}

	void LineStyle::parse(const std::string& input)
	{
	}

	LineSymbol::LineSymbol(const std::string& Reference, double position, double rotation, const std::string& crsType)
		: Reference(Reference), position(position), rotation(rotation), crsType(crsType) {}

	void LineSymbol::execute() 
	{
		// Execute LineSymbol
	}

	void LineSymbol::parse(const std::string& input)
	{
	}

	Dash::Dash(double start, double length)
		: start(start), length(length) {}

	void Dash::execute() 
	{
		// Execute Dash
	}
	void Dash::parse(const std::string& input)
	{
	}
}