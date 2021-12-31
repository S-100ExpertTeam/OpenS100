-- LightAirObstruction portrayal rules file.

-- Referenced CSPs.
require 'LightFlareAndDescription'

-- Main entry point for feature type.
function LightAirObstruction(feature, featurePortrayal, contextParameters)
	if contextParameters.RADAR_OVERLAY then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:8;DisplayPlane:OverRADAR')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:8;DisplayPlane:UnderRADAR')
	end

	LightFlareAndDescription(feature, featurePortrayal, contextParameters, 6)
end
