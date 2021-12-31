-- MooringTrot main entry point.

function MooringTrot(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.

	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') -- TODO: Find aggregates and emit instructions.

	return 21010
end
