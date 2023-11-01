-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Archipelagic sea lane area main entry point.
function ArchipelagicSeaLaneArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 26260
		featurePortrayal:AddInstructions('ViewingGroup:26260;DrawingPriority:15;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:ARCSLN01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
