function Dolphin(feature, featurePortrayal, contextParameters)
	local viewingGroup = 12410

	featurePortrayal:AddInstructions('AlertReference:NavHazard')
	featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:18')
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and Paper chart use same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		end
		
		-- #437
		local categoryOfDolphin = feature.categoryOfDolphin
		
		if (contains(2,categoryOfDolphin)) then
			-- Deviation Dolphin
			featurePortrayal:AddInstructions('PointInstruction:MORFAC04')
		else
			-- Mooring Dolphin
			featurePortrayal:AddInstructions('PointInstruction:MORFAC03')
		end
		

	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
