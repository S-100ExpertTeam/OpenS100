-- NavigationalSystemOfMarks portrayal rules file.

-- UNOFFICIAL:  Rules extracted from S-52 lookup table for M_NSYS.

-- Main entry point for feature type.
function NavigationalSystemOfMarks(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType ~= PrimitiveType.Surface then
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR')

	if contextParameters.PlainBoundaries then
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		featurePortrayal:AddInstructions('LineInstruction:MARSYS51')
	end

	return 27040
end
