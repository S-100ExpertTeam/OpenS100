#include "stdafx.h"
#include "AreaFillReference.h"

namespace DrawingCommand
{
    AreaFillReference::AreaFillReference(const std::string& reference) : reference(reference)
    {
    }

    void AreaFillReference::init()
    {
        Command::init();
        reference.clear();
    }

    void AreaFillReference::execute()
    {
    }

    void AreaFillReference::parse(const std::string& input)
    {
        setPresent();
        // AreaFillReference:reference 
        reference = input;
    }
}
