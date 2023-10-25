-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Rapids main entry point.
function Rapids(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRADAR;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR;NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32050
		featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHGRD')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
