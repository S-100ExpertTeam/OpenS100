#pragma once

#include "Command.h"

#include "..\\S100Engine\\GraphicBasePackage_Enum.h"

namespace DrawingInstructions
{
    class LineStyle : public Command 
    {
    public:
		LineStyle() = default;
        LineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency,
            const std::string& capStyle, const std::string& joinStyle, double offset);
		virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string name;
        double intervalLength = 0.0;
        double width = 0.0;
        std::string token;
        double transparency = 0.0;
        std::string capStyle = "Butt";
        std::string joinStyle = "Miter";
        double offset = 0.0;
    };

    class LineSymbol : public Command {
    public:
		LineSymbol() = default;
        LineSymbol(const std::string& Reference, double position, double rotation, const GraphicBasePackage::CRSType crsType);
		virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;
    private:
        std::string reference;
        double position = 0.0;
        double rotation = 0.0;
        GraphicBasePackage::CRSType crsType = GraphicBasePackage::CRSType::localCRS;
		double scaleFactor = 1.0; 
    };

    class Dash : public Command 
    {
    public:
		Dash() = default;
        Dash(double start, double length);
		virtual void init() override;
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;
    private:
        double start = 0.0;
        double length = 0.0;
    };
}

