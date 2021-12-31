-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Rescue station main entry point.
function RescueStation(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:7;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:7;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:RSCSTA02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
