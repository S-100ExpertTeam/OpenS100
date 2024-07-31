
-- Dry dock main entry point.
function DryDock(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32440
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:LANDA')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
