-- RestrictedAreaRegulatory main entry point.

function RestrictedAreaRegulatory(feature, featurePortrayal, contextParameters)
	-- TODO: Feature has no geometry and is an aggregation of other features.
	--       Determine if aggregated features need special handling beyond their standard portrayal.
	
	if contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,114,114')
	elseif not contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,104,104')
	end

	featurePortrayal:AddInstructions('NullInstruction')
end
