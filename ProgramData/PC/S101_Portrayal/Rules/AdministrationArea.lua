
-- Administration Area main entry point.
function AdministrationArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if (feature.PrimitiveType == PrimitiveType.Surface or feature.PrimitiveType == PrimitiveType.Curve) and contextParameters.PlainBoundaries then
		viewingGroup = 36050
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface or feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 36050
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('LineInstruction:ADMARE01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
