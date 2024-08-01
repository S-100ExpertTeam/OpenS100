function Helipad(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
	    featurePortrayal:AddInstructions('PointInstruction:HELIPD02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
