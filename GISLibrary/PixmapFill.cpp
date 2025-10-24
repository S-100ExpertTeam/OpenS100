#include "stdafx.h"
#include "PixmapFill.h"

namespace DrawingCommand
{
    // PixmapFill class implementation
    PixmapFill::PixmapFill(const std::string& reference) : reference(reference)
    {
    }

    void PixmapFill::init()
    {
        Command::init();
        reference.clear();
    }

    void PixmapFill::execute()
    {
    }

    void PixmapFill::parse(const std::string& input)
    {
        setPresent();
        // PixmapFill:reference 
        reference = input;
    }
}
