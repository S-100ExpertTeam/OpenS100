-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Cable area main entry point.
function CableArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 34030
		featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CBLARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 34030
		featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CBLARE51')
		featurePortrayal:AddInstructions('LineInstruction:CBLARE51')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
