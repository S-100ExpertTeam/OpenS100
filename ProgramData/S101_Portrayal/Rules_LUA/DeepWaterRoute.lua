-- Main entry point for feature type.
function DeepWaterRoute(feature, featurePortrayal, contextParameters)
	-- TODO: Determine portrayal rules if any.  This feature has no spatials, only feature associations.
	featurePortrayal:AddInstructions('NullInstruction')
end
