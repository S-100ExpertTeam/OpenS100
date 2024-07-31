
-- issue  #142, [PSWG #112]

-- Berth main entry point.
function Berth(feature, featurePortrayal, contextParameters)
	local viewingGroup

	local categoryOfCargo = feature.categoryOfCargo -- = 7  -- replace with updated FC with: categoryOfCargo = feature.categoryOfCargo

	local symbol = 'BRTHNO01'	-- default for categoryOfCargo != 7

	if contains(7, categoryOfCargo) then
		symbol = 'BRTHDNG1'
	end
		
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32440
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:' .. symbol) 
		featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 15)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32440
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 15)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32440
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
		featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 9)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
