#pragma once

#include "VectorParameter.h"
#include "Command.h"

#include <string>
#include <vector>

namespace DrawingInstructions
{
    // Base class for all drawing commands
  //  class DrawingCommand 
  //  {
  //  public:
		//DrawingCommand() = default;
  //      virtual ~DrawingCommand() = default;

  //  public:
  //      virtual void init();
  //      virtual void execute()  = 0;
  //      virtual void parse(const std::string& input) = 0;

  //  public:
		//bool present = false;

  //  public:
  //      void setPresent(bool value = true);
		//bool isPresent() const;
  //  };

    class PointInstruction : public Command
    {
    public:
		PointInstruction() = default;
        PointInstruction(const std::string& symbol);
		virtual ~PointInstruction() = default;
        
    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string symbol;

    public:
        std::string GetSymbol() const;
    };

    class LineInstruction : public Command
    {
    public:
        LineInstruction() = default;
        LineInstruction(const std::vector<std::string>& lineStyle);
		virtual ~LineInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };

    class LineInstructionUnsuppressed : public Command
    {
    public:
		LineInstructionUnsuppressed() = default;
        LineInstructionUnsuppressed(const std::vector<std::string>& lineStyle);
		virtual ~LineInstructionUnsuppressed() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> lineStyle;
    };

    class ColorFill : public Command
    {
    public:
		ColorFill() = default;
        ColorFill(const std::string& token, double transparency = 0.0);
		virtual ~ColorFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency = 0.0;
    };

    class AreaFillReference : public Command
    {
    public:
		AreaFillReference() = default;
        AreaFillReference(const std::string& reference);
		virtual ~AreaFillReference() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class PixmapFill : public Command
    {
    public:
		PixmapFill() = default;
        PixmapFill(const std::string& reference);
		virtual ~PixmapFill() = default;

    public:
        virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string reference;
    };

    class SymbolFill : public Command
    {
    public:
		SymbolFill() = default;
        SymbolFill(const std::string& symbol, const DrawingInstructions::Vector& v1, const DrawingInstructions::Vector& v2, bool clipSymbols = true);
		virtual ~SymbolFill() = default;

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

    class HatchFill : public Command
    {
    public:
		HatchFill() = default;
        HatchFill(const DrawingInstructions::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2);
		virtual ~HatchFill() = default;
        
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

    class TextInstruction : public Command
    {
    public:
		TextInstruction() = default;
        TextInstruction(const std::string& text);
		virtual ~TextInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string text;
    };

    class CoverageFill : public Command
    {
    public:
		CoverageFill() = default;
        CoverageFill(const std::string& attributeCode, const std::string& uom = "", const std::string& placement = "");
		virtual ~CoverageFill() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string attributeCode;
        std::string uom;
        std::string placement;
    };

    class NullInstruction : public Command
    {
    public:
		NullInstruction() = default;
		virtual ~NullInstruction() = default;

    public:
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;
    };
}