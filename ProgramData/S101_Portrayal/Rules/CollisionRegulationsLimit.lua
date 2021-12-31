-- CollisionRegulationsLimit portrayal rules file.

-- Main entry point for feature type.
function CollisionRegulationsLimit(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:testPCB')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
