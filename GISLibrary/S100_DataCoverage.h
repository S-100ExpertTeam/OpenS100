#pragma once
#include <memory>
#include <vector>
#include "S100_TemporalExtent.h"
#include "EX_BoundingPolygon.h"


namespace S100 {
	class S100_DataCoverage
	{
	public:
		EX_BoundingPolygon BoundingPolygon;
		std::shared_ptr<S100_TemporalExtent> TemporalExtent;
		std::shared_ptr<int> OptimumDisplayScale;
		std::shared_ptr<int> MaximumDisplayScale;
		std::shared_ptr<int> MinimumDisplayScale;
		std::vector<double> ApproximateGridResolution;



		inline void GetContents(pugi::xml_node& node)
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
					S100_TemporalExtent te;
					te.GetContents(instruction);
					TemporalExtent = std::make_shared<S100_TemporalExtent>(te);
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
				else if (!strcmp(instructionName, "S100XC:approximateGridResolution"))
				{
					ApproximateGridResolution.push_back(atof(instruction.child_value()));
				}
				else
				{
				}
			}
		}
	};

}