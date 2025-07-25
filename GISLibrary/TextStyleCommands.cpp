#include "stdafx.h"
#include "TextStyleCommands.h"

namespace DrawingInstructions
{
    TextStyleCommands::~TextStyleCommands()
    {
		delete fontColor;
		fontColor = nullptr;

		delete fontBackgroundColor;
		fontBackgroundColor = nullptr;

		delete fontSize;
		fontSize = nullptr;

		delete fontProportion;
		fontProportion = nullptr;

		delete fontWeight;
		fontWeight = nullptr;

		delete fontSlant;
		fontSlant = nullptr;

		delete fontSerifs;
		fontSerifs = nullptr;

		delete fontUnderline;
		fontUnderline = nullptr;

		delete fontStrikethrough;
		fontStrikethrough = nullptr;

		delete textAlignHorizontal;
		textAlignHorizontal = nullptr;

		delete textAlignVertical;
		textAlignVertical = nullptr;

		delete fontReference;
		fontReference = nullptr;

		delete fontUpperline;
		fontUpperline = nullptr;

		delete textVerticalOffset;
		textVerticalOffset = nullptr;
    }
    void TextStyleCommands::setFontColor(const std::string& token, double transparency) {
        delete this->fontColor;
        this->fontColor = new FontColor(token, transparency);
    }

    void TextStyleCommands::setFontBackgroundColor(const std::string& token, double transparency)
    {
        delete this->fontBackgroundColor;
		this->fontBackgroundColor = new FontBackgroundColor(token, transparency);
    }

    void TextStyleCommands::setFontSize(double bodySize) {
        delete this->fontSize;
        this->fontSize = new FontSize(bodySize);
    }

    void TextStyleCommands::setFontProportion(const std::string& proportion) {
        delete this->fontProportion;
        this->fontProportion = new FontProportion(proportion);
    }

    void TextStyleCommands::setFontWeight(const std::string& weight) {
        delete this->fontWeight;
        this->fontWeight = new FontWeight(weight);
    }

    void TextStyleCommands::setFontSlant(const std::string& slant) {
        delete this->fontSlant;
        this->fontSlant = new FontSlant(slant);
    }

    void TextStyleCommands::setFontSerifs(bool serifs) {
        delete this->fontSerifs;
        this->fontSerifs = new FontSerifs(serifs);
    }

    void TextStyleCommands::setFontUnderline(bool underline) {
        delete this->fontUnderline;
        this->fontUnderline = new FontUnderline(underline);
    }

    void TextStyleCommands::setFontStrikethrough(bool strikethrough) {
        delete this->fontStrikethrough;
        this->fontStrikethrough = new FontStrikethrough(strikethrough);
    }

    void TextStyleCommands::setFontUpperline(bool overline)
    {
        delete this->fontUpperline;
		this->fontUpperline = new FontUpperline(overline);
    }

    void TextStyleCommands::setFontReference(const std::string& reference)
    {
        delete this->fontReference;
        this->fontReference = new FontReference(reference);
    }

    void TextStyleCommands::setTextAlignHorizontal(const std::string& horizontalAlignment) {
        delete this->textAlignHorizontal;
        this->textAlignHorizontal = new TextAlignHorizontal(horizontalAlignment);
    }

    void TextStyleCommands::setTextAlignVertical(const std::string& verticalAlignment) {
        delete this->textAlignVertical;
        this->textAlignVertical = new TextAlignVertical(verticalAlignment);
    }

    void TextStyleCommands::setTextVerticalOffset(double verticalOffset)
    {
		delete this->textVerticalOffset;
		this->textVerticalOffset = new TextVerticalOffset(verticalOffset);
    }


	void TextStyleCommands::parse(const std::string& key, std::string value)
	{
		if (key == "FontColor")
		{
			//fontColor->execute();
		}
		else if (key == "FontSize")
		{
			//fontSize->execute();
		}
		else if (key == "FontProportion")
		{
			//fontProportion->execute();
		}
		else if (key == "FontWeight")
		{
			//fontWeight->execute();
		}
		else if (key == "FontSlant")
		{
			//fontSlant->execute();
		}
		else if (key == "FontSerifs")
		{
			//fontSerifs->execute();
		}
		else if (key == "FontUnderline")
		{
			//fontUnderline->execute();
		}
		else if (key == "FontStrikethrough")
		{
			//fontStrikethrough->execute();
		}
		else if (key == "TextAlignHorizontal")
		{
			//textAlignHorizontal->execute();
		}
		else if (key == "TextAlignVertical")
		{
			//textAlignVertical->execute();
		}	

	}

	void TextStyleCommands::execute() const
	{
        if (fontColor) fontColor->execute();
		if (fontSize) fontSize->execute();
		if (fontProportion) fontProportion->execute();
		if (fontWeight) fontWeight->execute();
		if (fontSlant) fontSlant->execute();
		if (fontSerifs) fontSerifs->execute();
		if (fontUnderline) fontUnderline->execute();
		if (fontStrikethrough) fontStrikethrough->execute();
		if (textAlignHorizontal) textAlignHorizontal->execute();
		if (textAlignVertical) textAlignVertical->execute();
		
	}



    // FontColor class implementation
    FontColor::FontColor(const std::string& token, double transparency) : token(token), transparency(transparency) {}

    void FontColor::init()
    {
        token.clear();
        transparency = 0.0;
	}

    void FontColor::execute()  {
    }

    void FontColor::parse(const std::string& input)
    {
    }

    FontBackgroundColor::FontBackgroundColor(const std::string& token, double transparency)
        : token(token), transparency(transparency)
    {
    }

    void FontBackgroundColor::init()
    {
        token.clear();
        transparency = 0.0;
	}

    void FontBackgroundColor::execute()  {
    }

    void FontBackgroundColor::parse(const std::string& input)
    {
    }


    // FontSize class implementation
    FontSize::FontSize(double bodySize) : bodySize(bodySize) {}

    void FontSize::init()
    {
        bodySize = 0.0;
	}

    void FontSize::execute()  {
    }

    void FontSize::parse(const std::string& input)
    {
    }

    // FontProportion class implementation
    FontProportion::FontProportion(const std::string& proportion) : proportion(proportion) {}

    void FontProportion::init()
    {
        proportion.clear();
    }

    void FontProportion::execute()  {
    }

    void FontProportion::parse(const std::string& input)
    {
    }

    // FontWeight class implementation
    FontWeight::FontWeight(const std::string& weight) : weight(weight) {}

    void FontWeight::init()
    {
        weight.clear();
	}

    void FontWeight::execute()  {
    }

    void FontWeight::parse(const std::string& input)
    {
    }

    // FontSlant class implementation
    FontSlant::FontSlant(const std::string& slant) : slant(slant) {}

    void FontSlant::init()
    {
        slant.clear();
	}

    void FontSlant::execute()  {
    }

    void FontSlant::parse(const std::string& input)
    {
    }

    // FontSerifs class implementation
    FontSerifs::FontSerifs(bool serifs) : serifs(serifs) {}

    void FontSerifs::init()
    {
        serifs = false; // Default value
    }

    void FontSerifs::execute()  {
    }

    void FontSerifs::parse(const std::string& input)
    {
    }

    // FontUnderline class implementation
    FontUnderline::FontUnderline(bool underline) : underline(underline) {}

    void FontUnderline::init()
    {
        underline = false; // Default value
	}

    void FontUnderline::execute()  {
    }

    void FontUnderline::parse(const std::string& input)
    {
    }

    // FontStrikethrough class implementation
    FontStrikethrough::FontStrikethrough(bool strikethrough) : strikethrough(strikethrough) {}

    void FontStrikethrough::init()
    {
        strikethrough = false; // Default value
    }

    void FontStrikethrough::execute()  {
    }

    void FontStrikethrough::parse(const std::string& input)
    {
    }

    // TextAlignHorizontal class implementation
    TextAlignHorizontal::TextAlignHorizontal(const std::string& horizontalAlignment) : horizontalAlignment(horizontalAlignment) {}

    void TextAlignHorizontal::init()
    {
        horizontalAlignment.clear();
    }

    void TextAlignHorizontal::execute()  {
    }

    void TextAlignHorizontal::parse(const std::string& input)
    {
    }

    // TextAlignVertical class implementation
    TextAlignVertical::TextAlignVertical(const std::string& verticalAlignment) : verticalAlignment(verticalAlignment) {}

    void TextAlignVertical::init()
    {
        verticalAlignment.clear();
	}

    void TextAlignVertical::execute()  {
    }

    void TextAlignVertical::parse(const std::string& input)
    {
    }

    void FontReference::init()
    {
        fontReference.clear();
	}

    void FontReference::execute() 
    {
    }

    void FontReference::parse(const std::string& input)
    {
    }

    void FontUpperline::init()
    {
        strikethrough = false; // Default value
	}

    void FontUpperline::execute() 
    {
    }

    void FontUpperline::parse(const std::string& input)
    {
    }

    void TextVerticalOffset::init()
    {
        verticalOffset = 0.0; // Default value
    }

    void TextVerticalOffset::execute() 
    {
    }

    void TextVerticalOffset::parse(const std::string& input)
    {
    }

}