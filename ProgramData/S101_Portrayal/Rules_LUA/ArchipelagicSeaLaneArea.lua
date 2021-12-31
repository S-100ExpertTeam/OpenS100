-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Archipelagic sea lane area main entry point.
function ArchipelagicSeaLaneArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:26260;DrawingPriority:5;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:ARCSLN01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
