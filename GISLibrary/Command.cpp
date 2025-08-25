#include "stdafx.h"
#include "Command.h"

namespace DrawingInstructions
{

    GraphicBasePackage::CRSType Command::GetCRSTypeFromString(const std::string& value)
    {
        if (value == "GeographicCRS")
        {
            return GraphicBasePackage::CRSType::geographicCRS;
        }
        else if (value == "PortrayalCRS")
        {
            return GraphicBasePackage::CRSType::portrayalCRS;
        }
        else if (value == "LocalCRS")
        {
            return GraphicBasePackage::CRSType::localCRS;
        }
        else if (value == "LineCRS")
        {
            return GraphicBasePackage::CRSType::lineCRS;
        }
        else
        {
            return GraphicBasePackage::CRSType::CRSType_None;
        }
    }

    IntervalType Command::GetIntervalTypeFromString(const std::string& value)
    {
        if (value == "openInterval")
        {
            return IntervalType::openInterval;
        }
        else if (value == "geLtInterval")
        {
            return IntervalType::geLtInterval;
        }
        else if (value == "gtLeInterval")
        {
            return IntervalType::gtLeInterval;
        }
        else if (value == "closedInterval")
        {
            return IntervalType::closedInterval;
        }
        else if (value == "gtSemiInterval")
        {
            return IntervalType::gtSemiInterval;
        }
        else if (value == "geSemiInterval")
        {
            return IntervalType::geSemiInterval;
        }
        else if (value == "ltSemiInterval")
        {
            return IntervalType::ltSemiInterval;
        }
        else if (value == "leSemiInterval")
        {
            return IntervalType::leSemiInterval;
		}
        else
        {
            return IntervalType::none;
        }
	}

    void Command::init()
    {
        present = false;
    }

    void Command::setPresent(bool value)
    {
        present = value;
    }

    bool Command::isPresent() const
    {
        return present;
    }
}