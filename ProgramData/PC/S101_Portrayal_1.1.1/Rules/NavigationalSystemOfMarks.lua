-- NavigationalSystemOfMarks portrayal rules file.
--
--  ISSUE: PSWG #42, PC	#47
--
-- UNOFFICIAL:  Rules extracted from S-52 lookup table for M_NSYS.

-- Main entry point for feature type.
function NavigationalSystemOfMarks(feature, featurePortrayal, contextParameters)

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Null instruction only for Pick Report support
		featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR;NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 27040
end
