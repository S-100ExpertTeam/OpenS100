-- DataCoverage portrayal rules file.

-- Main entry point for feature type.
function DataCoverage(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('ViewingGroup:31040;DrawingPriority:3;DisplayPlane:UnderRADAR;NullInstruction')

	return 31040
end
