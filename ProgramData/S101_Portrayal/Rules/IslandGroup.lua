-- Main entry point for feature type.
function IslandGroup(feature, featurePortrayal, contextParameters)
	-- TODO: Determine portrayal rules if any.  This feature has no spatials, only feature associations.
	featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:3;DisplayPlane:UnderRADAR;NullInstruction')

	return 12010
end
