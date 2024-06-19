#pragma once
#include <string>
#include <vector>

namespace DrawingCommands
{
    // Base class for all drawing commands
    class DrawingCommand {
    public:
        virtual ~DrawingCommand() = default;
        virtual void execute()  = 0;
        virtual void parse(const std::string& input) = 0;
    };

    class PointInstruction : public DrawingCommand {
    public:
        PointInstruction(const std::string& symbol);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
    };

    class LineInstruction : public DrawingCommand {
    public:
        LineInstruction(const std::string& lineStyle);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string lineStyle;
    };

    class LineInstructionUnsuppressed : public DrawingCommand {
    public:
        LineInstructionUnsuppressed(const std::string& lineStyle);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string lineStyle;
    };

    class ColorFill : public DrawingCommand {
    public:
        ColorFill(const std::string& token, double transparency = 0.0);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency;
    };

    class AreaFillReference : public DrawingCommand {
    public:
        AreaFillReference(const std::string& reference);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class PixmapFill : public DrawingCommand {
    public:
        PixmapFill(const std::string& reference);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class SymbolFill : public DrawingCommand {
    public:
        SymbolFill(const std::string& symbol, const std::vector<double>& v1, const std::vector<double>& v2, bool clipSymbols = true);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;
        std::vector<double> v1;
        std::vector<double> v2;
        bool clipSymbols;
    };

    class HatchFill : public DrawingCommand {
    public:
        HatchFill(const std::vector<double>& direction, double distance, const std::string& lineStyle);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<double> direction;
        double distance;
        std::string lineStyle;
    };

    class TextInstruction : public DrawingCommand {
    public:
        TextInstruction(const std::string& text);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string text;
    };

    class CoverageFill : public DrawingCommand {
    public:
        CoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string attributeCode;
        std::string uom;
        std::string placement;
    };

    class NullInstruction : public DrawingCommand {
    public:
        virtual void execute() override;
        virtual void parse(const std::string& input) override;
    };

    // DrawingCommands class
    class DrawingCommands {
    public:
        ~DrawingCommands();

        void setPointInstruction(const std::string& symbol);
        void setLineInstruction(const std::string& lineStyle);
        void setLineInstructionUnsuppressed(const std::string& lineStyle);
        void setColorFill(const std::string& token, double transparency = 0.0);
        void setAreaFillReference(const std::string& reference);
        void setPixmapFill(const std::string& reference);
        void setSymbolFill(const std::string& symbol, const std::vector<double>& v1, const std::vector<double>& v2, bool clipSymbols = true);
        void setHatchFill(const std::vector<double>& direction, double distance, const std::string& lineStyle);
        void setTextInstruction(const std::string& text);
        void setCoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");
        void setNullInstruction();

        void parse(const std::string& key, std::string value);
        void executeCommands() const;

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