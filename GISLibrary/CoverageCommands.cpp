#include "stdafx.h"
#include "CoverageCommands.h"

namespace DrawingInstructions {
	CoverageCommands::~CoverageCommands()
	{
		delete numericAnnotation;
		numericAnnotation = nullptr;

		delete symbolAnnotation;
		symbolAnnotation = nullptr;

		delete coverageColor;
		coverageColor = nullptr;

		delete lookupEntry;
		lookupEntry = nullptr;
	}
	void CoverageCommands::setNumericAnnotation(int decimals, const std::string& championChoice, double buffer)
	{
		delete this->numericAnnotation;
		this->numericAnnotation = new NumericAnnotation(decimals, championChoice, buffer);
	}

	void CoverageCommands::setSymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute, const std::string& rotationCRS, double rotationOffset, double rotationFactor, double scaleFactor)
	{
		delete this->symbolAnnotation;
		this->symbolAnnotation = new SymbolAnnotation(symbolRef, rotationAttribute, scaleAttribute, rotationCRS, rotationOffset, rotationFactor, scaleFactor);
	}

	void CoverageCommands::setCoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth)
	{
		delete this->coverageColor;
		this->coverageColor = new CoverageColor(startToken, startTransparency, endToken, endTransparency, penWidth);
	}

	void CoverageCommands::setLookupEntry(const std::string& label, double lower, double upper, const std::string& closure)
	{
		delete this->lookupEntry;
		this->lookupEntry = new LookupEntry(label, lower, upper, closure);
	}

	void CoverageCommands::parse(const std::string& key, std::string value)
	{
		if (key == "NumericAnnotation")
		{
			//numericAnnotation->execute();
		}
		else if (key == "SymbolAnnotation")
		{
			//symbolAnnotation->execute();
		}
		else if (key == "CoverageColor")
		{
			//coverageColor->execute();
		}
		else if (key == "LookupEntry")
		{
			//lookupEntry->execute();
		}
	}

	void CoverageCommands::execute() const
	{
		if (numericAnnotation) numericAnnotation->execute();
		if (symbolAnnotation) symbolAnnotation->execute();
		if (coverageColor) coverageColor->execute();
		if (lookupEntry) lookupEntry->execute();
	}

	NumericAnnotation::NumericAnnotation(int decimals, const std::string& championChoice, double buffer)
		: decimals(decimals), championChoice(championChoice), buffer(buffer) {}

	void NumericAnnotation::execute()  {
	}

	void NumericAnnotation::parse(const std::string& input)
	{
	}

	SymbolAnnotation::SymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute, const std::string& rotationCRS, double rotationOffset, double rotationFactor, double scaleFactor)
		: symbolRef(symbolRef), rotationAttribute(rotationAttribute), scaleAttribute(scaleAttribute), rotationCRS(rotationCRS), rotationOffset(rotationOffset), rotationFactor(rotationFactor), scaleFactor(scaleFactor) {}

	void SymbolAnnotation::execute()  {
	}

	void SymbolAnnotation::parse(const std::string& input)
	{
	}

	CoverageColor::CoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth)
		: startToken(startToken), startTransparency(startTransparency), endToken(endToken), endTransparency(endTransparency), penWidth(penWidth) {}

	void CoverageColor::execute()  {
	}

	void CoverageColor::parse(const std::string& input)
	{
	}

	LookupEntry::LookupEntry(const std::string& label, double lower, double upper, const std::string& closure)
		: label(label), lower(lower), upper(upper), closure(closure) {}

	void LookupEntry::execute()  {
	}
	void LookupEntry::parse(const std::string& input)
	{
	}
}
