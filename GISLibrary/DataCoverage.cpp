#include "stdafx.h"
#include "DataCoverage.h"

namespace S100 {
    void DataCoverage::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:boundingPolygon"))
            {
                EX_BoundingPolygon bp;
                bp.GetContents(instruction);
                BoundingPolygon = bp;
            }
            else if (!strcmp(instructionName, "S100XC:temporalExtent"))
            {
                TemporalExtent te;
                te.GetContents(instruction);
                temporalExtent = std::make_shared<TemporalExtent>(te);
            }
            else if (!strcmp(instructionName, "S100XC:optimumDisplayScale"))
            {
                OptimumDisplayScale = std::make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:maximumDisplayScale"))
            {
                MaximumDisplayScale = std::make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:minimumDisplayScale"))
            {
                MinimumDisplayScale = std::make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:apsproximateGridResolution"))
            {
                ApproximateGridResolution.push_back(atof(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:boundingBox"))
            {
                EX_GeographicBoundingBox gb;
                gb.GetContents(instruction);
                BoundingBox = std::make_shared<EX_GeographicBoundingBox>(gb);
            }
            else
            {
            }
        }
    }

    void DataCoverage::Save(pugi::xml_node& node)
    {
        {
            auto child = node.append_child("S100XC:boundingPolygon");
            BoundingPolygon.Save(child);
        }
        if(temporalExtent)
        {
            auto child = node.append_child("S100XC:temporalExtent");
            temporalExtent->Save(child);
        }
        if (OptimumDisplayScale)
        {
            auto child = node.append_child("S100XC:optimumDisplayScale");
            child.text().set(std::to_string(*OptimumDisplayScale).c_str());
        }
        if (MaximumDisplayScale)
        {
            auto child = node.append_child("S100XC:maximumDisplayScale");
            child.text().set(std::to_string(*MaximumDisplayScale).c_str());
        }
        if (MinimumDisplayScale)
        {
            auto child = node.append_child("S100XC:minimumDisplayScale");
            child.text().set(std::to_string(*MinimumDisplayScale).c_str());
        }
        if (!ApproximateGridResolution.empty())
        {
            for (int i = 0; i < ApproximateGridResolution.size(); i++)
            {
                auto child = node.append_child("S100XC:approximateGridResolution");
                child.text().set(std::to_string(ApproximateGridResolution[i]).c_str());
            }
        }
    }
}
