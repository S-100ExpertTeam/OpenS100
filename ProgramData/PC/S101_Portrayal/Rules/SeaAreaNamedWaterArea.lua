
-- Sea area/named water area main entry point.
function SeaAreaNamedWaterArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 21060
		featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRadar')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 9)
		end
		featurePortrayal:AddInstructions('NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 21060
		featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRadar')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 9)
		end
		featurePortrayal:AddInstructions('NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
