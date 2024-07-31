
-- Rapids main entry point.
function Rapids(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRadar;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar;NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32050
		featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:CHGRD')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
