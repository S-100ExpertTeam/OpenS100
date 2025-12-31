#include "stdafx.h"
#include "StateCommandStore.h"

bool StateCommandStore::SetStateCommand(Part9a::StateCommand* command)
{
    if (command == nullptr)
    {
        return false;
    }

    if (auto state = dynamic_cast<Part9a::ViewingGroup*>(command))
    {
        viewingGroup = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::DisplayPlane*>(command))
    {
        displayPlane = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::DrawingPriority*>(command))
    {
        drawingPriority = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ScaleMinimum*>(command))
    {
        scaleMinimum = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ScaleMaximum*>(command))
    {
        scaleMaximum = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Id*>(command))
    {
        id = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Parent*>(command))
    {
        parent = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Hover*>(command))
    {
        hover = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::LocalOffset*>(command))
    {
        localOffset = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::LinePlacement*>(command))
    {
        linePlacement = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AreaPlacement*>(command))
    {
        areaPlacement = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AreaCRS*>(command))
    {
        areaCRS = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Rotation*>(command))
    {
        rotation = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ScaleFactor*>(command))
    {
        scaleFactor = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Dash*>(command))
    {
        dash = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::LineSymbol*>(command))
    {
        lineSymbol = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::LineStyle*>(command))
    {
        lineStyle = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontColor*>(command))
    {
        fontColor = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontBackgroundColor*>(command))
    {
        fontBackgroundColor = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontSize*>(command))
    {
        fontSize = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontProportion*>(command))
    {
        fontProportion = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontWeight*>(command))
    {
        fontWeight = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontSlant*>(command))
    {
        fontSlant = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontSerifs*>(command))
    {
        fontSerifs = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontUnderline*>(command))
    {
        fontUnderline = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontStrikethrough*>(command))
    {
        fontStrikethrough = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontUpperline*>(command))
    {
        fontUpperline = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::FontReference*>(command))
    {
        fontReference = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::TextAlignHorizontal*>(command))
    {
        textAlignHorizontal = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::TextAlignVertical*>(command))
    {
        textAlignVertical = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::TextVerticalOffset*>(command))
    {
        textVerticalOffset = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::OverrideColor*>(command))
    {
        overrideColor = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::OverrideAll*>(command))
    {
        overrideAll = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ClearOverride*>(command))
    {
        overrideColor.reset();
        overrideAll.reset();
        clearOverride = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::SpatialReference*>(command))
    {
        spatialReference = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::TimeValid*>(command))
    {
        timeValid = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AlertReference*>(command))
    {
        alertReference = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AugmentedPoint*>(command))
    {
        augmentedPoint = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AugmentedRay*>(command))
    {
        augmentedRay = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::AugmentedPath*>(command))
    {
        augmentedPath = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Polyline*>(command))
    {
        polyline = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Arc3Points*>(command))
    {
        arc3Points = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ArcByRadius*>(command))
    {
        arcByRadius = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Annulus*>(command))
    {
        annulus = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ClearGeometry*>(command))
    {
        augmentedPoint.reset();
        augmentedRay.reset();
        augmentedPath.reset();
        polyline.reset();
        arc3Points.reset();
        arcByRadius.reset();
        annulus.reset();
        spatialReference.reset();
        clearGeometry = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::NumericAnnotation*>(command))
    {
        numericAnnotation = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::SymbolAnnotation*>(command))
    {
        symbolAnnotation = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::CoverageColor*>(command))
    {
        coverageColor = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::LookupEntry*>(command))
    {
        lookupEntry = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Date*>(command))
    {
        date = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::Time*>(command))
    {
        time = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::DateTime*>(command))
    {
        dateTime = *state;
        return true;
    }
    else if (auto state = dynamic_cast<Part9a::ClearTime*>(command))
    {
        date.reset();
        time.reset();
        dateTime.reset();
        timeValid.reset();
        clearTime = *state;
        return true;
    }

    return false;
}
