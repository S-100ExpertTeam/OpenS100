function Bollard(feature, featurePortrayal, contextParameters)
	local viewingGroup = 32440

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart uses same symbolization
		featurePortrayal:AddInstructions('AlertReference:NavHazard;ViewingGroup:' .. viewingGroup .. ';DrawingPriority:18')
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:PILPNT02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
