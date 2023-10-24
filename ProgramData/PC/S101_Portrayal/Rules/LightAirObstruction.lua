-- LightAirObstruction portrayal rules file.

-- Referenced CSPs.
require 'LightFlareAndDescription'

-- Main entry point for feature type.
function LightAirObstruction(feature, featurePortrayal, contextParameters)
	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:OverRADAR;Hover:true')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:UnderRADAR;Hover:true')
	end

	LightFlareAndDescription(feature, featurePortrayal, contextParameters, 6, 27070, 24)

	return 27070
end
