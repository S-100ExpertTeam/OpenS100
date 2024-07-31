
-- Referenced portrayal rules.
require 'DEPCNT03'

-- Depth contour main entry point.
function DepthContour(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 33020
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:15;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33020;DrawingPriority:15;DisplayPlane:UnderRadar')
		end
		DEPCNT03(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
