-- PilotageDistrict portrayal rules file.
--
-- ISSUES: PSWG #57, PC #94

-- Main entry point for feature type.
function PilotageDistrict(feature, featurePortrayal, contextParameters)
	local viewingGroup = 28010

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:ADMARE01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end


	return viewingGroup
end
