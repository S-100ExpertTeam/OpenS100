
-- Small craft facility main entry point.
function SmallCraftFacility(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 38210
		featurePortrayal:AddInstructions('ViewingGroup:38210;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 38210
		featurePortrayal:AddInstructions('ViewingGroup:38210;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
