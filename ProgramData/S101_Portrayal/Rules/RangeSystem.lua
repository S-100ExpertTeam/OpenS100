-- Main entry point for feature type.
function RangeSystem(feature, featurePortrayal, contextParameters)
	-- No portrayal needed.  Portrayal of components is sufficient.
	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') -- TODO: Find aggregates and emit instructions.

	return 21010
end
