-- LightFogDetector portrayal rules file.

-- Referenced CSPs.
require 'LightFlareAndDescription'

-- Main entry point for feature type.
function LightFogDetector(feature, featurePortrayal, contextParameters)
	if contextParameters.RADAR_OVERLAY then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:8;DisplayPlane:OverRADAR')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:8;DisplayPlane:UnderRADAR')
	end

	LightFlareAndDescription(feature, featurePortrayal, contextParameters, 7)
end
