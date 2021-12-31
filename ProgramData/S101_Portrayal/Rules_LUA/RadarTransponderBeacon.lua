-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Radar transponder beacon main entry point.
function RadarTransponderBeacon(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:27210;DrawingPriority:6;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27210;DrawingPriority:6;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:RTPBCN02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
