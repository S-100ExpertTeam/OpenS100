-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Retroreflector main entry point.
function Retroreflector(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:6;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:6;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:RETRFL02')
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:6;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:6;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:RETRFL01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
