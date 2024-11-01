
-- Radar transponder beacon main entry point.
function RadarTransponderBeacon(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 27210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27210;DrawingPriority:18;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27210;DrawingPriority:18;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:RTPBCN02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
