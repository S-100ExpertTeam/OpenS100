
-- Referenced portrayal rules.
require 'RESTRN01'

-- Precautionary area main entry point.
function PrecautionaryArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 25010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:15;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:15;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:PRCARE12')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 25010
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:PRCARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 25010
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('AreaFillReference:TSSJCT02')
		featurePortrayal:AddInstructions('PointInstruction:PRCARE51')
		featurePortrayal:AddInstructions('LineInstruction:PRCARE51')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
