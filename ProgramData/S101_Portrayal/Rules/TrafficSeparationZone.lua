-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Traffic separation zone main entry point.
function TrafficSeparationZone(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('AlertReference:ProhAre,102,102')

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:TRFCF,0.75')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
