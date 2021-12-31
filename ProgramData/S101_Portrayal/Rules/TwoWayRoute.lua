-- TwoWayRoute main entry point.

function TwoWayRoute(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.

	featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR;NullInstruction')

	return 25010
end
