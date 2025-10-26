#pragma once
#include "StateCommand.h"

namespace DrawingCommand
{
    class OverrideColor : public StateCommand
    {
    public:
		OverrideColor() = default;
        OverrideColor(const std::string& colorToken, double colorTransparency, const std::string& overrideToken, double overrideTransparency);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string colorToken;
        double colorTransparency = 0.0;
        std::string overrideToken;
        double overrideTransparency = 0.0;
    };

    class OverrideAll : public StateCommand
    {
    public:
		OverrideAll() = default;
        OverrideAll(const std::string& token, double transparency);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency = 0.0;
    };

    class ClearOverride : public StateCommand
	{
    public:
        ClearOverride() = default;

    public:
        void init() override;
        void execute() override;
        void parse(const std::string& input) override;
	};
}

