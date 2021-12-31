-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Coastguard station main entry point.
function CoastguardStation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 38030
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:21;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:21;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CGUSTA02')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		--TODO: Determine S-101 portrayal for CoastguardStation surface feature.
		viewingGroup = 21010
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;PointInstruction:testPCB')
		Debug.Trace('Warning: S-52 does not define portrayal for CoastguardStation surface features.')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
