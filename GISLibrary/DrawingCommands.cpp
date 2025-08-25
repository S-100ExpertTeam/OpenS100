#include "stdafx.h"
#include "DrawingCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
    /*void DrawingCommand::init()
    {
        present = false;
    }

    void DrawingCommand::setPresent(bool value)
    {
		present = value;
    }

    bool DrawingCommand::isPresent() const
    {
		return present;
    }*/

    // PointInstruction class implementation
    PointInstruction::PointInstruction(const std::string& symbol) : symbol(symbol) 
    {
    }

	void PointInstruction::init()
	{
        Command::init();
        symbol.clear();
	}

    void PointInstruction::execute()  
    {
    }

    void PointInstruction::parse(const std::string& input)
    {
        setPresent();
        // PointInstruction:symbol 
        symbol = input;
    }

    std::string PointInstruction::GetSymbol() const
    {
		return symbol;
    }

    // LineInstruction class implementation
    LineInstruction::LineInstruction(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle)
    {
    }

    void LineInstruction::init()
    {
        Command::init();
        lineStyle.clear();
	}

    void LineInstruction::execute() 
    {
    }

    void LineInstruction::parse(const std::string& input)
    {
        setPresent();
        // LineInstruction:lineStyle[,lineStyle,¡¦] 
		lineStyle = LatLonUtility::Split(input, ",");
    }

    // LineInstructionUnsuppressed class implementation
    LineInstructionUnsuppressed::LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle) {}

    void LineInstructionUnsuppressed::init()
    {
        Command::init();
        lineStyle.clear();
    }

    void LineInstructionUnsuppressed::execute()  
    {
    }

    void LineInstructionUnsuppressed::parse(const std::string& input)
    {
        setPresent();
        // LineInstructionUnsuppressed:lineStyle[,lineStyle,¡¦] 
        lineStyle = LatLonUtility::Split(input, ",");
    }

    // ColorFill class implementation
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
				transparency = std::stod(tokens[1]);
			}
			catch (...) 
            {
				transparency = 0.0; // Default value in case of error
			}
		}
    }

    // AreaFillReference class implementation
    AreaFillReference::AreaFillReference(const std::string& reference) : reference(reference) 
    {
    }

	void AreaFillReference::init()
	{
        Command::init();
		reference.clear();
	}

    void AreaFillReference::execute()  
    {
    }

    void AreaFillReference::parse(const std::string& input)
    {
        setPresent();
        // AreaFillReference:reference 
		reference = input;
    }

    // PixmapFill class implementation
    PixmapFill::PixmapFill(const std::string& reference) : reference(reference) 
    {
    }

	void PixmapFill::init()
	{
        Command::init();
		reference.clear();
	}

    void PixmapFill::execute()  
    {
    }

    void PixmapFill::parse(const std::string& input)
    {
        setPresent();
        // PixmapFill:reference 
		reference = input;
    }

    // SymbolFill class implementation
    SymbolFill::SymbolFill(const std::string& symbol, const DrawingInstructions::Vector& v1, const DrawingInstructions::Vector& v2, bool clipSymbols)
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

    void SymbolFill::execute()  {
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

    // HatchFill class implementation
    HatchFill::HatchFill(const DrawingInstructions::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2)
        : direction(direction), distance(distance), lineStyle1(lineStyle1), lineStyle2(lineStyle2) {}

    void HatchFill::init()
    {
        Command::init();
        direction.Set(0.0, 0.0);
        distance = 0.0;
        lineStyle1.clear();
        lineStyle2.clear();
	}

    void HatchFill::execute()  {
    }

    void HatchFill::parse(const std::string& input)
    {
        setPresent();
        // HatchFill:direction,distance,lineStyle[,lineStyle] 
		auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() >= 4)
        {
            try
            {
				direction.Set(std::stod(tokens[0]), std::stod(tokens[1]));
				distance = std::stod(tokens[2]);
				lineStyle1 = tokens[3];

				if (tokens.size() == 5) 
                {
					lineStyle2 = tokens[4];
				}
			}
            catch (...) {
                init();
            }
        }
    }

    // TextInstruction class implementation
    TextInstruction::TextInstruction(const std::string& text) : text(text) {}

    void TextInstruction::init()
    {
        Command::init();
        text.clear();
	}

    void TextInstruction::execute()  {
    }

    void TextInstruction::parse(const std::string& input)
    {
        setPresent();
        // TextInstruction:text 
		text = input;
    }

    // CoverageFill class implementation
    CoverageFill::CoverageFill(const std::string& attributeCode, const std::string& uom, const std::string& placement)
        : attributeCode(attributeCode), uom(uom), placement(placement) {}

    void CoverageFill::init()
    {
        Command::init();
        attributeCode.clear();
        uom.clear();
        placement.clear();
    }

    void CoverageFill::execute()  
    {
    }

    void CoverageFill::parse(const std::string& input)
    {
        setPresent();
        // CoverageFill:attributeCode[,uom[,placement]] 
		auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() > 0) 
        {
            attributeCode = tokens[0];
            if (tokens.size() > 1) 
            {
                uom = tokens[1];
                if (tokens.size() > 2) 
                {
                    placement = tokens[2];
                }
            }
		}
        else 
        {
            init(); // Reset to default values
        }
    }

    void NullInstruction::init()
    {
        Command::init();
        // No specific initialization needed for NullInstruction
	}

    // NullInstruction class implementation
    void NullInstruction::execute()  {
    }

    void NullInstruction::parse(const std::string& input)
    {
        setPresent();
        // NullInstruction 
    }
}