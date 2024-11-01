
-- Tidal stream panel data main entry point.
function TidalStreamPanelData(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 33060
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:TIDSTR01')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 33060
		featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:TIDSTR01')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 33060
		featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:6;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:TIDSTR01')
		featurePortrayal:AddInstructions('LineInstruction:TIDINF51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
