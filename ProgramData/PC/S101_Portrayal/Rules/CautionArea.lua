
-- Caution Area main entry point.
function CautionArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53017,53017')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26150
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO06')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26150
		featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26150
		featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTNARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
