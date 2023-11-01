-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'DEPCNT03'

-- Depth contour main entry point.
function DepthContour(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 33020
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		DEPCNT03(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
