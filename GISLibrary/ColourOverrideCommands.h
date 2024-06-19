#pragma once
#include "StateCommand.h"

namespace DrawingInstruction
{
    class OverrideColor : public StateCommand {
    public:
        OverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string colorToken;
        double colorTransparency;
        std::string overrideToken;
        double overrideTransparency;
    };

    class OverrideAll : public StateCommand {
    public:
        OverrideAll(const std::string& token, double transparency);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency;
    };

	class ColourOverrideCommands
	{
	public:
		ColourOverrideCommands() = default;
		ColourOverrideCommands(const ColourOverrideCommands&) = delete;
		ColourOverrideCommands& operator=(const ColourOverrideCommands&) = delete;
		ColourOverrideCommands(ColourOverrideCommands&&) = delete;
		ColourOverrideCommands& operator=(ColourOverrideCommands&&) = delete;
		~ColourOverrideCommands();

		void setOverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency);
        void setOverrideAll(const std::string& token, double transparency);
        void clearOverride();


        void parse(const std::string& key, std::string value);
        void executeCommands() const;

    private:
        //Colour Override
		OverrideColor* overrideColor = nullptr;
		OverrideAll* overrideAll = nullptr;
	};
}

