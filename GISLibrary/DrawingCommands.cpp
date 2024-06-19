#include "stdafx.h"
#include "DrawingCommands.h"


namespace DrawingInstructions
{
    // PointInstruction class implementation
    PointInstruction::PointInstruction(const std::string& symbol) : symbol(symbol) {}

    void PointInstruction::execute()  {
    }

    void PointInstruction::parse(const std::string& input)
    {
    }

    // LineInstruction class implementation
    LineInstruction::LineInstruction(const std::string& lineStyle) : lineStyle(lineStyle) {}

    void LineInstruction::execute() {
    }

    void LineInstruction::parse(const std::string& input)
    {
    }

    // LineInstructionUnsuppressed class implementation
    LineInstructionUnsuppressed::LineInstructionUnsuppressed(const std::string& lineStyle) : lineStyle(lineStyle) {}

    void LineInstructionUnsuppressed::execute()  {
    }

    void LineInstructionUnsuppressed::parse(const std::string& input)
    {
    }

    // ColorFill class implementation
    ColorFill::ColorFill(const std::string& token, double transparency) : token(token), transparency(transparency) {}

    void ColorFill::execute()  {
    }

    void ColorFill::parse(const std::string& input)
    {
    }

    // AreaFillReference class implementation
    AreaFillReference::AreaFillReference(const std::string& reference) : reference(reference) {}

    void AreaFillReference::execute()  {
    }

    void AreaFillReference::parse(const std::string& input)
    {
    }

    // PixmapFill class implementation
    PixmapFill::PixmapFill(const std::string& reference) : reference(reference) {}

    void PixmapFill::execute()  {
    }

    void PixmapFill::parse(const std::string& input)
    {
    }

    // SymbolFill class implementation
    SymbolFill::SymbolFill(const std::string& symbol, const std::vector<double>& v1, const std::vector<double>& v2, bool clipSymbols)
        : symbol(symbol), v1(v1), v2(v2), clipSymbols(clipSymbols) {}

    void SymbolFill::execute()  {
    }

    void SymbolFill::parse(const std::string& input)
    {
    }

    // HatchFill class implementation
    HatchFill::HatchFill(const std::vector<double>& direction, double distance, const std::string& lineStyle)
        : direction(direction), distance(distance), lineStyle(lineStyle) {}

    void HatchFill::execute()  {
    }

    void HatchFill::parse(const std::string& input)
    {
    }

    // TextInstruction class implementation
    TextInstruction::TextInstruction(const std::string& text) : text(text) {}

    void TextInstruction::execute()  {
    }

    void TextInstruction::parse(const std::string& input)
    {
    }

    // CoverageFill class implementation
    CoverageFill::CoverageFill(const std::string& attributeCode, const std::string& uom, const std::string& placement)
        : attributeCode(attributeCode), uom(uom), placement(placement) {}

    void CoverageFill::execute()  {
    }

    void CoverageFill::parse(const std::string& input)
    {
    }

    // NullInstruction class implementation
    void NullInstruction::execute()  {
    }

    void NullInstruction::parse(const std::string& input)
    {
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

    void DrawingCommands::setLineInstruction(const std::string& lineStyle) {
        delete lineInstruction;
        lineInstruction = new LineInstruction(lineStyle);
    }

    void DrawingCommands::setLineInstructionUnsuppressed(const std::string& lineStyle) {
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

    void DrawingCommands::setSymbolFill(const std::string& symbol, const std::vector<double>& v1, const std::vector<double>& v2, bool clipSymbols) {
        delete symbolFill;
        symbolFill = new SymbolFill(symbol, v1, v2, clipSymbols);
    }

    void DrawingCommands::setHatchFill(const std::vector<double>& direction, double distance, const std::string& lineStyle) {
        delete hatchFill;
        hatchFill = new HatchFill(direction, distance, lineStyle);
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