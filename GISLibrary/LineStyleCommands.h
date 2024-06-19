#pragma once
#include "StateCommand.h"

namespace DrawingInstruction
{
    class LineStyle : public StateCommand {
    public:
        LineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency,
            const std::string& capStyle, const std::string& joinStyle, double offset);
        virtual void execute()  override;
        virtual void parse(const std::string& input) override;

    private:
        std::string name;
        double intervalLength;
        double width;
        std::string token;
        double transparency;
        std::string capStyle;
        std::string joinStyle;
        double offset;
    };

    class LineSymbol : public StateCommand {
    public:
        LineSymbol(const std::string& Reference, double position, double rotation, const std::string& crsType);

        virtual void execute()  override;
        virtual void parse(const std::string& input) override;
    private:
        std::string Reference;
        double position;
        double rotation;
        std::string crsType;
    };

    class Dash : public StateCommand {
    public:
        Dash(double start, double length);

        virtual void execute()  override;
        virtual void parse(const std::string& input) override;
    private:
        double start;
        double length;
    };

	class LineStyleCommands
	{
		public:
		LineStyleCommands() = default;
		LineStyleCommands(const LineStyleCommands&) = delete;
		LineStyleCommands& operator=(const LineStyleCommands&) = delete;
		LineStyleCommands(LineStyleCommands&&) = delete;
		LineStyleCommands& operator=(LineStyleCommands&&) = delete;
		~LineStyleCommands();

 		void setLineStyle(const std::string& name, double intervalLength, double width, const std::string& token, double transparency,
            const std::string& capStyle, const std::string& joinStyle, double offset);
        void setLineSymbol(const std::string& Reference, double position, double rotation, const std::string& crsType);
        void setDash(double start, double length);


        void parse(const std::string& key, std::string value);
        void executeCommands() const;
    private:
          //Line Style
        LineStyle* lineStyle = nullptr;
        LineSymbol* lineSymbol = nullptr;
        Dash* dash = nullptr;
	};
}

