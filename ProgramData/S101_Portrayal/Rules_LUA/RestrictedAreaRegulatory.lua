-- RestrictedAreaRegulatory main entry point.

function RestrictedAreaRegulatory(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.
	
	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')
end
