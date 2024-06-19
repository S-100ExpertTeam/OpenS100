#pragma once
#include "StateCommand.h"

namespace DrawingInstructions
{
    class FontColor : public StateCommand {
    public:
        FontColor(const std::string& token, double transparency);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string token;
        double transparency;
    };

    class FontBackgroundColor : public StateCommand {
    public:
        FontBackgroundColor(const std::string& token, double transparency);
        void execute() override;
        void parse(const std::string& input) override;
    private:
        std::string token;
        double transparency;
    };

    class FontSize : public StateCommand {
    public:
        FontSize(double bodySize);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double bodySize;
    };

    class FontProportion : public StateCommand {
    public:
        FontProportion(const std::string& proportion);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string proportion;
    };

    class FontWeight : public StateCommand {
    public:
        FontWeight(const std::string& weight);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string weight;
    };

    class FontSlant : public StateCommand {
    public:
        FontSlant(const std::string& slant);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string slant;
    };

    class FontSerifs : public StateCommand {
    public:
        FontSerifs(bool serifs);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool serifs;
    };

    class FontUnderline : public StateCommand {
    public:
        FontUnderline(bool underline);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool underline;
    };

    class FontStrikethrough : public StateCommand {
    public:
        FontStrikethrough(bool strikethrough);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool strikethrough;
    };

    class FontUpperline : public StateCommand {
    public:
        FontUpperline(bool strikethrough)
            : strikethrough(strikethrough) {}
        void execute() override;
        void parse(const std::string& input) override;

    private:
        bool strikethrough;
    };

    class FontReference : public StateCommand {
    public:
        FontReference(const std::string& fontReference)
            : fontReference(fontReference) {}
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string fontReference;
    };

    class TextAlignHorizontal : public StateCommand {
    public:
        TextAlignHorizontal(const std::string& horizontalAlignment);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string horizontalAlignment;
    };

    class TextAlignVertical : public StateCommand {
    public:
        TextAlignVertical(const std::string& verticalAlignment);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string verticalAlignment;
    };

    class TextVerticalOffset : public StateCommand {
    public:
        TextVerticalOffset(double verticalOffset)
            : verticalOffset(verticalOffset) {}
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double verticalOffset;
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

