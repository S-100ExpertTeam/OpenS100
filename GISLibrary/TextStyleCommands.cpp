#include "stdafx.h"
#include "TextStyleCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
    // FontColor class implementation
    FontColor::FontColor(const std::string& token, double transparency) : token(token), transparency(transparency) {}

    void FontColor::init()
    {
        Command::init();
        token.clear();
        transparency = 0.0;
	}

    void FontColor::execute()  {
    }

    void FontColor::parse(const std::string& input)
    {
        setPresent();
        // FontColor:token[,transparency] 
		auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() > 0) 
        {
            token = tokens[0];

            if (tokens.size() > 1) 
            {
                try 
                {
                    transparency = std::stod(tokens[1]);
                }
                catch (const std::exception&) 
                {
                    transparency = 0.0; // Default value if parsing fails
                }
            }
            else 
            {
                transparency = 0.0; // Default value
            }
        }
        else
        {
            init();
        }
    }

    FontBackgroundColor::FontBackgroundColor(const std::string& token, double transparency)
        : token(token), transparency(transparency)
    {
    }

    void FontBackgroundColor::init()
    {
        Command::init();
        token.clear();
        transparency = 1.0;
	}

    void FontBackgroundColor::execute()  {
    }

    void FontBackgroundColor::parse(const std::string& input)
    {
        setPresent();
        // FontBackgroundColor:token[,transparency] 

        auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() > 0)
        {
            token = tokens[0];

            if (tokens.size() > 1)
            {
                try
                {
                    transparency = std::stod(tokens[1]);
                }
                catch (const std::exception&)
                {
                    transparency = 0.0; // Default value if parsing fails
                }
            }
            else
            {
                transparency = 0.0; // Default value
            }
        }
        else
        {
            init();
        }
    }


    // FontSize class implementation
    FontSize::FontSize(double bodySize) : bodySize(bodySize) {}

    void FontSize::init()
    {
        Command::init();
        bodySize = 10.0;
	}

    void FontSize::execute()  {
    }

    void FontSize::parse(const std::string& input)
    {
        setPresent();
        // FontSize:bodySize 
        try
        {
            bodySize = std::stod(input);
        }
        catch (const std::exception& e)
        {
			init(); // Reset to default value if parsing fails
		}
    }

    // FontProportion class implementation
    FontProportion::FontProportion(const std::string& proportion) : proportion(proportion) {}

    void FontProportion::init()
    {
        Command::init();
        proportion = "Proportional";
    }

    void FontProportion::execute()  {
    }

    void FontProportion::parse(const std::string& input)
    {
        setPresent();
        // FontProportion:proportion 
        if (input == "Proportional" || input == "MonoSpaced") 
        {
            proportion = input;
        } 
        else 
        {
            init(); // Reset to default value if parsing fails
		}
    }

    // FontWeight class implementation
    FontWeight::FontWeight(const std::string& weight) : weight(weight) {}

    void FontWeight::init()
    {
        Command::init();
        weight = "Medium";
	}

    void FontWeight::execute()  {
    }

    void FontWeight::parse(const std::string& input)
    {
        setPresent();
        // FontWeight:weight
        if (input == "Light" ||
            input == "Medium" || 
            input == "Bold") 
        {
            weight = input;
        } 
        else
        {
            init(); // Reset to default value if parsing fails
        }
    }

    // FontSlant class implementation
    FontSlant::FontSlant(const std::string& slant) : slant(slant) {}

    void FontSlant::init()
    {
        Command::init();
        slant = "Upright";
	}

    void FontSlant::execute()  {
    }

    void FontSlant::parse(const std::string& input)
    {
        setPresent();
        // FontSlant:slant
        if (input == "Upright" || 
            input == "Italics") 
        {
            slant = input;
        } 
        else
        {
            init(); // Reset to default value if parsing fails
        }
    }

    // FontSerifs class implementation
    FontSerifs::FontSerifs(bool serifs) : serifs(serifs) {}

    void FontSerifs::init()
    {
        Command::init();
        serifs = false; // Default value
    }

    void FontSerifs::execute()  {
    }

    void FontSerifs::parse(const std::string& input)
    {
        setPresent();
        // FontSerifs:serifs 
        if (input == "true" || input == "false") 
        {
            serifs = (input == "true");
        } 
        else 
        {
            init(); // Reset to default value if parsing fails
		}
    }

    // FontUnderline class implementation
    FontUnderline::FontUnderline(bool underline) : underline(underline) {}

    void FontUnderline::init()
    {
        Command::init();
        underline = false; // Default value
	}

    void FontUnderline::execute()  {
    }

    void FontUnderline::parse(const std::string& input)
    {
        setPresent();
        // FontUnderline:underline 
        if (input == "true" || input == "false") 
        {
            underline = (input == "true");
        } 
        else
        {
            init(); // Reset to default value if parsing fails
        }
    }

    // FontStrikethrough class implementation
    FontStrikethrough::FontStrikethrough(bool strikethrough) : strikethrough(strikethrough) {}

    void FontStrikethrough::init()
    {
        Command::init();
        strikethrough = false; // Default value
    }

    void FontStrikethrough::execute()  {
    }

    void FontStrikethrough::parse(const std::string& input)
    {
        setPresent();
        // FontStrikethrough:strikethrough 
        if (input == "true" || input == "false") 
        {
            strikethrough = (input == "true");
        } 
        else
        {
            init(); // Reset to default value if parsing fails
        }
    }

    // TextAlignHorizontal class implementation
    TextAlignHorizontal::TextAlignHorizontal(const std::string& horizontalAlignment) : horizontalAlignment(horizontalAlignment) {}

    void TextAlignHorizontal::init()
    {
        Command::init();
        horizontalAlignment = "Start";
    }

    void TextAlignHorizontal::execute()  {
    }

    void TextAlignHorizontal::parse(const std::string& input)
    {
        setPresent();
        // TextAlignHorizontal:horizontalAlignment 
        if (input == "Start" || 
            input == "Center" || 
            input == "End") 
        {
            horizontalAlignment = input;
        } 
        else
        {
            init(); // Reset to default value if parsing fails
		}
    }

    // TextAlignVertical class implementation
    TextAlignVertical::TextAlignVertical(const std::string& verticalAlignment) : verticalAlignment(verticalAlignment) {}

    void TextAlignVertical::init()
    {
        Command::init();
        verticalAlignment = "Bottom";
	}

    void TextAlignVertical::execute()  {
    }

    void TextAlignVertical::parse(const std::string& input)
    {
        setPresent();
        // TextAlignVertical:verticalAlignment 
		if (input == "Top" ||
			input == "Center" ||
			input == "Bottom")
		{
			verticalAlignment = input;
		}
		else
		{
			init(); // Reset to default value if parsing fails
		}
    }

    void FontReference::init()
    {
        Command::init();
        fontReference.clear();
	}

    void FontReference::execute() 
    {
    }

    void FontReference::parse(const std::string& input)
    {
        setPresent();
		// FontReference:fontReference 
		fontReference = input;
    }

    void FontUpperline::init()
    {
        Command::init();
        strikethrough = false;
	}

    void FontUpperline::execute() 
    {
    }

    void FontUpperline::parse(const std::string& input)
    {
        setPresent();
        // FontUpperline:upperline 
        if (input == "true" || input == "false") 
        {
            strikethrough = (input == "true");
        } 
        else
        {
            init(); // Reset to default value if parsing fails
        }
    }

    void TextVerticalOffset::init()
    {
        Command::init();
        verticalOffset = 0.0; // Default value
    }

    void TextVerticalOffset::execute() 
    {
    }

    void TextVerticalOffset::parse(const std::string& input)
    {
        setPresent();
        // TextVerticalOffset:verticalOffset 
        try
        {
            verticalOffset = std::stod(input);
        }
        catch (const std::exception& e)
        {
            init(); // Reset to default value if parsing fails
		}
    }

}