
-- River main entry point.
function River(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12420
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12420
		featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:DEPVS')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
