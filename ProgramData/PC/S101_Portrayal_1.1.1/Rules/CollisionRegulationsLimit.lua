-- CollisionRegulationsLimit portrayal rules file.
--
-- ISSUES: PSWG #41, PC #109
--
-- Main entry point for feature type.
function CollisionRegulationsLimit(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		-- TODO: needs proper viewing group assignment etc
		viewingGroup = 25001

		featurePortrayal:AddInstructions('ViewingGroup:25001;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:COLREG01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
