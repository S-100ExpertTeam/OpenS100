-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'DEPARE03'

-- Depth area main entry point.
function DepthArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 13030
		featurePortrayal:AddInstructions('ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRADAR')
		DEPARE03(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
