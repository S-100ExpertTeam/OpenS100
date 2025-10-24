#include "stdafx.h"
#include "SymbolFill.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingCommand
{
	// SymbolFill class implementation
	SymbolFill::SymbolFill(const std::string& symbol, const DrawingCommand::Vector& v1, const DrawingCommand::Vector& v2, bool clipSymbols)
		: symbol(symbol), v1(v1), v2(v2), clipSymbols(clipSymbols)
	{
	}

	void SymbolFill::init()
	{
		Command::init();
		symbol.clear();
		v1.Set(0.0, 0.0);
		v2.Set(0.0, 0.0);
		clipSymbols = true;
	}

	void SymbolFill::execute() {
	}

	void SymbolFill::parse(const std::string& input)
	{
		setPresent();
		// SymbolFill:symbol,v1,v2[,clipSymbols] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() >= 5)
		{
			symbol = tokens[0];
			try {
				v1.Set(std::stod(tokens[1]), std::stod(tokens[2]));
				v2.Set(std::stod(tokens[3]), std::stod(tokens[4]));
			}
			catch (...) {
				v1.Set(0.0, 0.0);
				v2.Set(0.0, 0.0);
			}
			if (tokens.size() > 5) {
				clipSymbols = (tokens[5] == "true");
			}
			else {
				clipSymbols = true; // Default value
			}
		}
		else
		{
			init();
		}
	}
}
