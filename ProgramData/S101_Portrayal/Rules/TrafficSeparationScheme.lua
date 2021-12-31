-- Main entry point for feature type.
function TrafficSeparationScheme(feature, featurePortrayal, contextParameters)
	-- TODO: Determine portrayal rules if any.  This feature has no spatials, only feature associations.
	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') -- TODO: Find aggregates and emit instructions.

	return 21010
end
