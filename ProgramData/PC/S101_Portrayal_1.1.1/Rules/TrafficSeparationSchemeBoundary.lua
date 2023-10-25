-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Traffic separation scheme boundary main entry point.
function TrafficSeparationSchemeBoundary(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 25010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:21;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:21;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
