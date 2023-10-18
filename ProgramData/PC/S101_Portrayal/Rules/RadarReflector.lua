-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Radar reflector main entry point.
function RadarReflector(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 27230
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27230;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27230;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:RADRFL03')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
