#pragma once
#include "StateCommand.h"

namespace DrawingInstructions
{
    class FontColor : public StateCommand {
    public:
		FontColor() = default;
        FontColor(const std::string& token, double transparency);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency = 0.0;
    };

    class FontBackgroundColor : public StateCommand {
    public:
		FontBackgroundColor() = default;
        FontBackgroundColor(const std::string& token, double transparency);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;
    private:
        std::string token;
        double transparency = 1.0;
    };

    class FontSize : public StateCommand 
    {
    public:
		FontSize() = default;
        FontSize(double bodySize);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double bodySize = 10.0;
    };

    class FontProportion : public StateCommand 
    {
    public:
		FontProportion() = default;
        FontProportion(const std::string& proportion);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string proportion = "Proportional";
    };

    class FontWeight : public StateCommand 
    {
    public:
		FontWeight() = default;
        FontWeight(const std::string& weight);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string weight = "Medium";
    };

    class FontSlant : public StateCommand 
    {
    public:
		FontSlant() = default;
        FontSlant(const std::string& slant);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string slant = "Upright";
    };

    class FontSerifs : public StateCommand 
    {
    public:
		FontSerifs() = default;
        FontSerifs(bool serifs);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool serifs = false;
    };

    class FontUnderline : public StateCommand 
    {
    public:
        FontUnderline() = default;
        FontUnderline(bool underline);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool underline = false;
    };

    class FontStrikethrough : public StateCommand 
    {
    public:
		FontStrikethrough() = default;
        FontStrikethrough(bool strikethrough);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool strikethrough = false;
    };

    class FontUpperline : public StateCommand 
    {
    public:
		FontUpperline() = default;
        FontUpperline(bool strikethrough)
            : strikethrough(strikethrough) {}

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool strikethrough = false;
    };

    class FontReference : public StateCommand 
    {
    public:
		FontReference() = default;
        FontReference(const std::string& fontReference)
            : fontReference(fontReference) {}

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string fontReference;
    };

    class TextAlignHorizontal : public StateCommand 
    {
    public:
		TextAlignHorizontal() = default;
        TextAlignHorizontal(const std::string& horizontalAlignment);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string horizontalAlignment = "Start";
    };

    class TextAlignVertical : public StateCommand 
    {
    public:
		TextAlignVertical() = default;
        TextAlignVertical(const std::string& verticalAlignment);

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string verticalAlignment = "Bottom";
    };

    class TextVerticalOffset : public StateCommand 
    {
    public:
		TextVerticalOffset() = default;
        TextVerticalOffset(double verticalOffset)
            : verticalOffset(verticalOffset) {}

    public:
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double verticalOffset = 0.0;
    };


	class TextStyleCommands
	{
    public:
        TextStyleCommands() = default;
        TextStyleCommands(const TextStyleCommands&) = delete;
        TextStyleCommands& operator=(const TextStyleCommands&) = delete;
        TextStyleCommands(TextStyleCommands&&) = delete;
        TextStyleCommands& operator=(TextStyleCommands&&) = delete;
        ~TextStyleCommands();

        void setFontColor(const std::string& token, double transparency);
        void setFontBackgroundColor(const std::string& token, double transparency);
        void setFontSize(double bodySize);
        void setFontProportion(const std::string& proportion);
        void setFontWeight(const std::string& weight);
        void setFontSlant(const std::string& slant);
        void setFontSerifs(bool serifs);
        void setFontUnderline(bool underline);
        void setFontStrikethrough(bool strikethrough);
        void setFontUpperline(bool overline);
        void setFontReference(const std::string& reference);
        void setTextAlignHorizontal(const std::string& horizontalAlignment);
        void setTextAlignVertical(const std::string& verticalAlignment);
        void setTextVerticalOffset(double verticalOffset);

        void parse(const std::string& key, std::string value);
        void execute() const;

    private:
        FontColor* fontColor = nullptr;
        FontBackgroundColor* fontBackgroundColor = nullptr;
        FontSize* fontSize = nullptr;
        FontProportion* fontProportion = nullptr;
        FontWeight* fontWeight = nullptr;
        FontSlant* fontSlant = nullptr;
        FontSerifs* fontSerifs = nullptr;
        FontUnderline* fontUnderline = nullptr;
        FontStrikethrough* fontStrikethrough = nullptr;
        FontUpperline* fontUpperline = nullptr; 
        FontReference* fontReference = nullptr;
        TextAlignHorizontal* textAlignHorizontal = nullptr;
        TextAlignVertical* textAlignVertical = nullptr;
        TextVerticalOffset* textVerticalOffset = nullptr;
	};

}

