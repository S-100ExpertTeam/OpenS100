-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Cargo transhipment area main entry point.
function CargoTranshipmentArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:3;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:26250;DrawingPriority:3;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
