-- QualityOfBathymetricData portrayal rules file.

-- Main entry point for feature type.
function QualityOfBathymetricData(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')
end
