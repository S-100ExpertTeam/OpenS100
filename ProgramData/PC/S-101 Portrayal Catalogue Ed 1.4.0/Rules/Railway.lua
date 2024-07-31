
-- Railway main entry point.
function Railway(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32250
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32250;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32250;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.64,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
