
-- Continental shelf area main entry point.
function ContinentalShelfArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface or feature.PrimitiveType == PrimitiveType.Curve then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 36010
		featurePortrayal:AddInstructions('ViewingGroup:36010;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
