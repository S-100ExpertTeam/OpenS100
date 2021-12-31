-- WindTurbine portrayal rules file.

-- Main entry point for feature type.
function WindTurbine(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		if feature.inTheWater then
			featurePortrayal:AddInstructions('ViewingGroup:12200')
		else
			featurePortrayal:AddInstructions('ViewingGroup:22220')
		end

		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('DrawingPriority:6;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('DrawingPriority:6;DisplayPlane:UnderRADAR')
		end

		featurePortrayal:AddInstructions('PointInstruction:testPCB')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
