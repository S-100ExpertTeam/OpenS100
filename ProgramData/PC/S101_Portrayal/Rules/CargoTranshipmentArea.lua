
-- Cargo transhipment area main entry point.
function CargoTranshipmentArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26250
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26250
		featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26250
		featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
