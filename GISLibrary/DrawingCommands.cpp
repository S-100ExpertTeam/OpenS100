#include "stdafx.h"
#include "DrawingCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
    // PointInstruction class implementation
    PointInstruction::PointInstruction(const std::string& symbol) : symbol(symbol) 
    {
    }

	void PointInstruction::init()
	{
        symbol.clear();
	}

    void PointInstruction::execute()  
    {
    }

    void PointInstruction::parse(const std::string& input)
    {
        // PointInstruction:symbol 
        symbol = input;
    }

    // LineInstruction class implementation
    LineInstruction::LineInstruction(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle)
    {
    }

    void LineInstruction::execute() 
    {
    }

    void LineInstruction::parse(const std::string& input)
    {
        // LineInstruction:lineStyle[,lineStyle,¡¦] 
		lineStyle = LatLonUtility::Split(input, ",");
    }

    // LineInstructionUnsuppressed class implementation
    LineInstructionUnsuppressed::LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle) : lineStyle(lineStyle) {}

    void LineInstructionUnsuppressed::execute()  {
    }

    void LineInstructionUnsuppressed::parse(const std::string& input)
    {
        // LineInstructionUnsuppressed:lineStyle[,lineStyle,¡¦] 
        lineStyle = LatLonUtility::Split(input, ",");
    }

    // ColorFill class implementation
    ColorFill::ColorFill(const std::string& token, double transparency) : token(token), transparency(transparency) 
    {
    }

	void ColorFill::init()
	{
		token.clear();
		transparency = 0.0;
	}

    void ColorFill::execute()  
    {
    }

    void ColorFill::parse(const std::string& input)
    {
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
		reference.clear();
	}

    void AreaFillReference::execute()  
    {
    }

    void AreaFillReference::parse(const std::string& input)
    {
        // AreaFillReference:reference 
		reference = input;
    }

    // PixmapFill class implementation
    PixmapFill::PixmapFill(const std::string& reference) : reference(reference) 
    {
    }

	void PixmapFill::init()
	{
		reference.clear();
	}

    void PixmapFill::execute()  
    {
    }

    void PixmapFill::parse(const std::string& input)
    {
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
		symbol.clear();
        v1.Set(0.0, 0.0);
        v2.Set(0.0, 0.0);
		clipSymbols = true;
	}

    void SymbolFill::execute()  {
    }

    void SymbolFill::parse(const std::string& input)
    {
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

    void HatchFill::execute()  {
    }

    void HatchFill::parse(const std::string& input)
    {
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

    void TextInstruction::execute()  {
    }

    void TextInstruction::parse(const std::string& input)
    {
        // TextInstruction:text 
		text = input;
    }

    // CoverageFill class implementation
    CoverageFill::CoverageFill(const std::string& attributeCode, const std::string& uom, const std::string& placement)
        : attributeCode(attributeCode), uom(uom), placement(placement) {}

    void CoverageFill::execute()  {
    }

    void CoverageFill::parse(const std::string& input)
    {
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

    // NullInstruction class implementation
    void NullInstruction::execute()  {
    }

    void NullInstruction::parse(const std::string& input)
    {
        // NullInstruction 
    }

    // DrawingCommands class implementation
    DrawingCommands::~DrawingCommands() {
        delete pointInstruction;
        delete lineInstruction;
        delete lineInstructionUnsuppressed;
        delete colorFill;
        delete areaFillReference;
        delete pixmapFill;
        delete symbolFill;
        delete hatchFill;
        delete textInstruction;
        delete coverageFill;
        delete nullInstruction;
    }

    void DrawingCommands::setPointInstruction(const std::string& symbol) {
        delete pointInstruction;
        pointInstruction = new PointInstruction(symbol);
    }

    void DrawingCommands::setLineInstruction(const std::vector<std::string>& lineStyle) {
        delete lineInstruction;
        lineInstruction = new LineInstruction(lineStyle);
    }

    void DrawingCommands::setLineInstructionUnsuppressed(const std::vector<std::string>& lineStyle) {
        delete lineInstructionUnsuppressed;
        lineInstructionUnsuppressed = new LineInstructionUnsuppressed(lineStyle);
    }

    void DrawingCommands::setColorFill(const std::string& token, double transparency) {
        delete colorFill;
        colorFill = new ColorFill(token, transparency);
    }

    void DrawingCommands::setAreaFillReference(const std::string& reference) {
        delete areaFillReference;
        areaFillReference = new AreaFillReference(reference);
    }

    void DrawingCommands::setPixmapFill(const std::string& reference) {
        delete pixmapFill;
        pixmapFill = new PixmapFill(reference);
    }

    void DrawingCommands::setSymbolFill(const std::string& symbol, const DrawingInstructions::Vector& v1, const DrawingInstructions::Vector& v2, bool clipSymbols) {
        delete symbolFill;
        symbolFill = new SymbolFill(symbol, v1, v2, clipSymbols);
    }

    void DrawingCommands::setHatchFill(const DrawingInstructions::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2) {
        delete hatchFill;
        hatchFill = new HatchFill(direction, distance, lineStyle1, lineStyle2);
    }

    void DrawingCommands::setTextInstruction(const std::string& text) {
        delete textInstruction;
        textInstruction = new TextInstruction(text);
    }

    void DrawingCommands::setCoverageFill(const std::string& attributeCode, const std::string& uom, const std::string& placement) {
        delete coverageFill;
        coverageFill = new CoverageFill(attributeCode, uom, placement);
    }

    void DrawingCommands::setNullInstruction() {
        delete nullInstruction;
        nullInstruction = new NullInstruction();
    }

    void DrawingCommands::parse(const std::string& key, std::string value)
    {
        if(key =="PointInstruction")
            pointInstruction->parse(key);
        else if (key == "LineInstruction")
			lineInstruction->parse(key);
		else if (key == "LineInstructionUnsuppressed")
			lineInstructionUnsuppressed->parse(key);
		else if (key == "ColorFill")
			colorFill->parse(key);
		else if (key == "AreaFillReference")
			areaFillReference->parse(key);
		else if (key == "PixmapFill")
			pixmapFill->parse(key);
		else if (key == "SymbolFill")
			symbolFill->parse(key);
		else if (key == "HatchFill")
			hatchFill->parse(key);
		else if (key == "TextInstruction")
			textInstruction->parse(key);
		else if (key == "CoverageFill")
			coverageFill->parse(key);
		else if (key == "NullInstruction")
			nullInstruction->parse(key);
    }

    void DrawingCommands::execute() const {
        if (pointInstruction) pointInstruction->execute();
        if (lineInstruction) lineInstruction->execute();
        if (lineInstructionUnsuppressed) lineInstructionUnsuppressed->execute();
        if (colorFill) colorFill->execute();
        if (areaFillReference) areaFillReference->execute();
        if (pixmapFill) pixmapFill->execute();
        if (symbolFill) symbolFill->execute();
        if (hatchFill) hatchFill->execute();
        if (textInstruction) textInstruction->execute();
        if (coverageFill) coverageFill->execute();
        if (nullInstruction) nullInstruction->execute();
    }
}