#include "stdafx.h"
#include "CoverageCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
	NumericAnnotation::NumericAnnotation(int decimals, const std::string& championChoice, double buffer)
		: decimals(decimals), championChoice(championChoice), buffer(buffer) {}

	void NumericAnnotation::init() 
	{
		Command::init();
		decimals = 0;
		championChoice = "ChampionChoice";
		buffer = 0.0;
	}

	void NumericAnnotation::execute()  {
	}

	void NumericAnnotation::parse(const std::string& input)
	{
		setPresent();
		// NumericAnnotation:decimals,championChoice[,buffer] 
		auto tokens = LatLonUtility::Split(input, ",");

		if (tokens.size() >= 2 && tokens.size() <= 3)
		{
			try {
				decimals = std::stoi(tokens[0]);
			}
			catch (const std::exception&) {
				decimals = 0; // Default value if parsing fails
			}
			
			championChoice = tokens[1];
			
			if (tokens.size() == 3)
			{
				try {
					buffer = std::stod(tokens[2]);
				}
				catch (const std::exception&) {
					buffer = 0.0; // Default value if parsing fails
				}
			}
		}
		else
		{
			init(); // Reset to default values if input is invalid
		}
	}

	SymbolAnnotation::SymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute, const GraphicBasePackage::CRSType rotationCRS, double rotationOffset, double rotationFactor, double scaleFactor)
		: symbolRef(symbolRef), rotationAttribute(rotationAttribute), scaleAttribute(scaleAttribute), rotationCRS(rotationCRS), rotationOffset(rotationOffset), rotationFactor(rotationFactor), scaleFactor(scaleFactor) {}

	void SymbolAnnotation::init()
	{
		Command::init();
		symbolRef.clear();
		rotationAttribute.clear();
		scaleAttribute.clear();
		rotationCRS = GraphicBasePackage::CRSType::CRSType_None;
		rotationOffset = 0.0;
		rotationFactor = 1.0;
		scaleFactor = 1.0;
	}

	void SymbolAnnotation::execute()  {
	}

	void SymbolAnnotation::parse(const std::string& input)
	{
		setPresent();
		// SymbolAnnotation:symbolRef,rotationAttribute,scaleAttribute[,rotationCRS,rotationOffset[,rotationFactor[,scaleFactor]]] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() >= 3 && tokens.size() <= 7)
		{
			symbolRef = tokens[0];
			rotationAttribute = tokens[1];
			scaleAttribute = tokens[2];
			if (tokens.size() > 3)
			{
				rotationCRS = Command::GetCRSTypeFromString(tokens[3]);
			}
			else
			{
				rotationCRS = GraphicBasePackage::CRSType::CRSType_None;
			}

			if (tokens.size() > 4)
			{
				try {
					rotationOffset = std::stod(tokens[4]);
				}
				catch (const std::exception&) {
					rotationOffset = 0.0; // Default value if parsing fails
				}
			}
			else
			{
				rotationOffset = 0.0;
			}

			if (tokens.size() > 5)
			{
				try {
					rotationFactor = std::stod(tokens[5]);
				}
				catch (const std::exception&) {
					rotationFactor = 1.0; // Default value if parsing fails
				}
			}
			else
			{
				rotationFactor = 1.0;
			}

			if (tokens.size() > 6)
			{
				try {
					scaleFactor = std::stod(tokens[6]);
				}
				catch (const std::exception&) {
					scaleFactor = 1.0; // Default value if parsing fails
				}
			}
		}
		else
		{
			init(); // Reset to default values if input is invalid
		}
	}

	CoverageColor::CoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth)
		: startToken(startToken), startTransparency(startTransparency), endToken(endToken), endTransparency(endTransparency), penWidth(penWidth) {}

	void CoverageColor::init()
	{
		Command::init();
		startToken.clear();
		startTransparency = 0.0;
		endToken.clear();
		endTransparency = 0.0;
		penWidth = 0.0; 
	}

	void CoverageColor::execute()  {
	}

	void CoverageColor::parse(const std::string& input)
	{
		setPresent();
		// CoverageColor:startToken,startTransparency[,endToken,endTransparency][,penWidth] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() >= 2 && tokens.size() <= 5)
		{
			startToken = tokens[0];
			try {
				startTransparency = std::stod(tokens[1]);
			}
			catch (const std::exception&) {
				startTransparency = 0.0; // Default value if parsing fails
			}

			if (tokens.size() > 2)
			{
				endToken = tokens[2];
				if (tokens.size() > 3)
				{
					try {
						endTransparency = std::stod(tokens[3]);
					}
					catch (const std::exception&) {
						endTransparency = 0.0; // Default value if parsing fails
					}
				}
				else
				{
					endTransparency = 0.0;
				}

				if (tokens.size() == 5)
				{
					try {
						penWidth = std::stod(tokens[4]);
					}
					catch (const std::exception&) {
						penWidth = 0.0; // Default value if parsing fails
					}
				}
				else
				{
					penWidth = 0.0;
				}
			}
			else
			{
				endToken.clear();
				endTransparency = 0.0;
				penWidth = 0.0;
			}
		}
		else
		{
			init(); // Reset to default values if input is invalid
		}
	}

	LookupEntry::LookupEntry(const std::string& label, double lower, double upper, const IntervalType closure)
		: label(label), lower(lower), upper(upper), closure(closure) {}

	void LookupEntry::init()
	{
		Command::init();
		label.clear();
		lower = 0.0;
		upper = 0.0;
		closure = IntervalType::none;
	}

	void LookupEntry::execute()  {
	}
	void LookupEntry::parse(const std::string& input)
	{
		setPresent();
		// LookupEntry:label,lower,upper,closure 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 4)
		{
			label = tokens[0];
			try {
				lower = std::stod(tokens[1]);
			}
			catch (const std::exception&) {
				lower = 0.0; // Default value if parsing fails
			}
			try {
				upper = std::stod(tokens[2]);
			}
			catch (const std::exception&) {
				upper = 0.0; // Default value if parsing fails
			}
			
			closure = Command::GetIntervalTypeFromString(tokens[3]);
		}
		else
		{
			init(); // Reset to default values if input is invalid
		}
	}
}
