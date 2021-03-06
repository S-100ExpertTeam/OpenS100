-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Traffic separation scheme crossing main entry point.
function TrafficSeparationSchemeCrossing(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('AreaFillReference:TSSJCT02')
		featurePortrayal:AddInstructions('PointInstruction:TSSCRS51')
		RESTRN01(feature, featurePortrayal, contextParameters)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
