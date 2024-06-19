#include "stdafx.h"
#include "StateCommands.h"




namespace DrawingInstructions
{
    // StateCommands class implementation
    StateCommands::~StateCommands() {
    }

    void StateCommands::parse(const std::string& key, std::string value)
    {
        if (key == "ViewingGroup" || key == "DisplayPlane"
            || key == "DrawingPriority" || key == "ScaleMinimum"
            || key == "ScaleMaximum" || key == "Id"
            || key == "Parent" || key == "Hover")
        {
            visibilityCommands->parse(key,value);
        }
        else if (key == "LocalOffset" || key == "LinePlacement"
            || key == "AreaPlacement" || key == "AreaCRS"
            || key == "Rotation" || key == "ScaleFactor")
        {
            transformCommands->parse(key,value);
        }
        else if (key == "LineStyle" || key == "LineSymbol"
            || key == "Dash")
        {
            lineStyleCommands->parse(key,value);
        }
        else if (key == "FontColor" || key == "FontSize"
            || key == "FontProportion" || key == "FontWeight"
            || key == "FontSlant" || key == "FontSerifs"
            || key == "FontUnderline" || key == "FontStrikethrough"
            || key == "FontUpperline" || key == "FontReference"
            || key == "TextAlignHorizontal" || key == "TextAlignVertical"
            || key == "TextVerticalOffset")
        {
            textStyleCommands->parse(key,value);
        }
        else if (key == "OverrideColor" || key == "OverrideAll")
        {
            colourOverrideCommands->parse(key,value);
        }
        else if (key == "SpatialReference" || key == "AugmentedPoint"
            || key == "AugmentedRay" || key == "AugmentedPath"
            || key == "Polyline" || key == "Arc3Points"
            || key == "ArcByRadius" || key == "Annulus"
            || key == "ClearGeometry")
        {
            geometryCommands->parse(key,value);
        }
        else if (key == "LookupEntry" || key == "NumericAnnotation"
            || key == "SymbolAnnotation" || key == "CoverageColor")
        {
            coverageCommands->parse(key, value);
        }
        else if (key == "Date" || key == "Time"
            || key == "DateTime" || key == "TimeValid"
            || key == "ClearTime")
        {
            timeCommands->parse(key, value);
        }
        else if (key == "AlertReference")
        {
            alertCommands->parse(key,value);
        }
    }

    void StateCommands::execute() {
		if (visibilityCommands)visibilityCommands->execute();
		if (transformCommands)transformCommands->execute();
        if (lineStyleCommands)lineStyleCommands->execute();
        if (textStyleCommands)textStyleCommands->execute();
        if (colourOverrideCommands)colourOverrideCommands->execute();
        if (geometryCommands)geometryCommands->execute();
        if (coverageCommands)coverageCommands->execute();
        if (timeCommands)timeCommands->execute();
        if (alertCommands)alertCommands->execute();
    }
}