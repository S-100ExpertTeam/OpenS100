-- ArchipelagicSeaLane main entry point.

function ArchipelagicSeaLane(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.

	featurePortrayal:AddInstructions('ViewingGroup:26260;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction')

	return 26260
end
