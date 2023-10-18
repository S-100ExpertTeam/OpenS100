-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Anchor berth main entry point.
function AnchorBerth(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26220
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:ACHBRT07')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 26220, 15)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 26220
		featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:15;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:ACHBRT07')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 26220, 15)
		end
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
