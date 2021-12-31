-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'DEPCNT03'

-- Depth contour main entry point.
function DepthContour(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:5;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:5;DisplayPlane:UnderRADAR')
		end
		DEPCNT03(feature, featurePortrayal, contextParameters)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
