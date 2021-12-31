-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Signal Station Traffic main entry point.
function SignalStationTraffic(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 28020
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:28020;DrawingPriority:21;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:28020;DrawingPriority:21;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:SISTAT02')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		--TODO: Determine S-101 portrayal for SignalStationTraffic surface feature.
		--TODO: Determine S-101 portrayal for SignalStationTraffic surface feature.
		--viewingGroup = 21010
		--featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;PointInstruction:testPCB')
		--Debug.Trace('Warning: S-52 does not define portrayal for SignalStationTraffic surface features.')

		viewingGroup = 28020
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:28020;DrawingPriority:21;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:28020;DrawingPriority:21;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:2021_SISTAT02_01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
