
-- Built-up area main entry point.
function BuiltUpArea(feature, featurePortrayal, contextParameters)
	local viewingGroup = 22240			-- Standard
	local InTheWater = false

	-- #365
	if feature.inTheWater == true then
		viewingGroup = 12410			--set to Base
		InTheWater = true
		featurePortrayal:AddInstructions('AlertReference:SafetyContour')	-- as per ShorelineConstruction
	end


	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:9;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:9;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:BUAARE02')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22240, 9)
		end		
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization

		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:9;DisplayPlane:UnderRadar')
			
		if InTheWater == false then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			
		else
			featurePortrayal:AddInstructions('ColorFill:CHGRF,0.5')
			featurePortrayal:AddInstructions('LineInstruction:LOWACC21')
		end

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22240, 9)
		end

	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
