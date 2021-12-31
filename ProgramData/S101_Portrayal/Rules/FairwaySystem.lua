-- FairwaySystem main entry point.

function FairwaySystem(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.

	featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR;NullInstruction')

	return 26050
end
