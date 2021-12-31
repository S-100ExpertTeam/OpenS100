-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Traffic separation scheme boundary main entry point.
function TrafficSeparationSchemeBoundary(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:7;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:7;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
