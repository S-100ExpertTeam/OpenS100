#pragma once

#include "VectorParameter.h"

#include <string>
#include <vector>

namespace DrawingInstructions
{
    // Base class for all drawing commands
    class DrawingCommand 
    {
    public:
		DrawingCommand() = default;
        virtual ~DrawingCommand() = default;

    public:
        virtual void init() = 0;
        virtual void execute()  = 0;
        virtual void parse(const std::string& input) = 0;
    };

    class PointInstruction : public DrawingCommand 
    {
    public:
        PointInstruction(const std::string& symbol);
        
    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
    };

    class LineInstruction : public DrawingCommand 
    {
    public:
        LineInstruction() = default;
        LineInstruction(const std::vector<std::string>& lineStyle);

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };

    class LineInstructionUnsuppressed : public DrawingCommand 
    {
    public:
		LineInstructionUnsuppressed() = default;
        LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle);

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };

    class ColorFill : public DrawingCommand 
    {
    public:
		ColorFill() = default;
        ColorFill(const std::string& token, double transparency = 0.0);

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency = 0.0;
    };

    class AreaFillReference : public DrawingCommand 
    {
    public:
		AreaFillReference() = default;
        AreaFillReference(const std::string& reference);

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class PixmapFill : public DrawingCommand 
    {
    public:
		PixmapFill() = default;
        PixmapFill(const std::string& reference);

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class SymbolFill : public DrawingCommand 
    {
    public:
		SymbolFill() = default;
        SymbolFill(const std::string& symbol, const DrawingInstructions::Vector& v1, const DrawingInstructions::Vector& v2, bool clipSymbols = true);

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
        DrawingInstructions::Vector v1;
        DrawingInstructions::Vector v2;
        bool clipSymbols = true;
    };

    class HatchFill : public DrawingCommand 
    {
    public:
		HatchFill() = default;
        HatchFill(const DrawingInstructions::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2);
        
    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        DrawingInstructions::Vector direction;
        double distance = 0.0;
        std::string lineStyle1;
		std::string lineStyle2;
    };

    class TextInstruction : public DrawingCommand 
    {
    public:
		TextInstruction() = default;
        TextInstruction(const std::string& text);

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string text;
    };

    class CoverageFill : public DrawingCommand 
    {
    public:
		CoverageFill() = default;
        CoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string attributeCode;
        std::string uom;
        std::string placement;
    };

    class NullInstruction : public DrawingCommand 
    {
    public:
		NullInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;
    };

    // DrawingCommands class
    class DrawingCommands {
    public:
        ~DrawingCommands();

        void setPointInstruction(const std::string& symbol);
        void setLineInstruction(const std::vector<std::string>& lineStyle);
        void setLineInstructionUnsuppressed(const std::vector<std::string>& lineStyle);
        void setColorFill(const std::string& token, double transparency = 0.0);
        void setAreaFillReference(const std::string& reference);
        void setPixmapFill(const std::string& reference);
        void setSymbolFill(const std::string& symbol, const DrawingInstructions::Vector& v1, const DrawingInstructions::Vector& v2, bool clipSymbols = true);
        void setHatchFill(const DrawingInstructions::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2);
        void setTextInstruction(const std::string& text);
        void setCoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");
        void setNullInstruction();

        void parse(const std::string& key, std::string value);
        void execute() const;

    private:
        PointInstruction* pointInstruction = nullptr;
        LineInstruction* lineInstruction = nullptr;
        LineInstructionUnsuppressed* lineInstructionUnsuppressed = nullptr;
        ColorFill* colorFill = nullptr;
        AreaFillReference* areaFillReference = nullptr;
        PixmapFill* pixmapFill = nullptr;
        SymbolFill* symbolFill = nullptr;
        HatchFill* hatchFill = nullptr;
        TextInstruction* textInstruction = nullptr;
        CoverageFill* coverageFill = nullptr;
        NullInstruction* nullInstruction = nullptr;
    };
}