-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Traffic separation line main entry point.
function TrafficSeparationLine(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 25010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:24;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:24;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',1.92,'TRFCF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
