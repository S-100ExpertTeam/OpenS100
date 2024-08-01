-- #176
-- #134

-- Runway main entry point.
function Runway(feature, featurePortrayal, contextParameters)
	local viewingGroup
	
	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:18;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:18;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32240
		featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:15;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
