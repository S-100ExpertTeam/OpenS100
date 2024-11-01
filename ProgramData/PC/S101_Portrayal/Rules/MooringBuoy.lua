function MooringBuoy(feature, featurePortrayal, contextParameters)
	local viewingGroup = 27010

	if feature.PrimitiveType == PrimitiveType.Point then
		featurePortrayal:AddInstructions('AlertReference:NavHazard;ViewingGroup:' .. viewingGroup .. ';DrawingPriority:24')
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		end
		
		if contextParameters.SimplifiedSymbols then
			featurePortrayal:AddInstructions('PointInstruction:BOYMOR11')
		elseif feature.buoyShape == 3 then
			featurePortrayal:AddInstructions('PointInstruction:BOYMOR01')
		elseif feature.buoyShape == 6 then
			featurePortrayal:AddInstructions('PointInstruction:BOYMOR03')
		else
			featurePortrayal:AddInstructions('PointInstruction:BOYMOR11')
		end
		
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
