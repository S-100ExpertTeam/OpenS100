
-- Traffic separation scheme boundary main entry point.
function TrafficSeparationSchemeBoundary(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 25010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:21;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:21;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
