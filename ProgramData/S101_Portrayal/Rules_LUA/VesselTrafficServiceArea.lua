-- VesselTrafficServiceArea portrayal rules file.

-- Main entry point for feature type.
function VesselTrafficServiceArea(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')

end
