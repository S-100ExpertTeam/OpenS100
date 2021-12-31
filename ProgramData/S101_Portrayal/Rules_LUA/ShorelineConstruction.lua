-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'SLCONS04'

-- Shoreline construction main entry point.
function ShorelineConstruction(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:MORFAC03')
		SLCONS04(feature, featurePortrayal, contextParameters)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:7;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:7;DisplayPlane:UnderRADAR')
		end
		SLCONS04(feature, featurePortrayal, contextParameters)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:7;DisplayPlane:UnderRADAR')
		SLCONS04(feature, featurePortrayal, contextParameters)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
