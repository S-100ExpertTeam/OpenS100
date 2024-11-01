-- #155
-- #238 [PSWG #117]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Buoy Safe Water main entry point.
function SafeWaterBuoy(feature, featurePortrayal, contextParameters)
	local viewingGroup = 27010
	local textViewingGroup = 21
	local priority = 24
	local textPriority = 24
	local displayPlane = 'UnderRadar'

	if contextParameters.RadarOverlay then
		displayPlane = 'OverRadar'
	end
	
	featurePortrayal:AddInstructions('AlertReference:NavHazard;Hover:true;ViewingGroup:' .. viewingGroup .. ';DrawingPriority:' .. priority .. ';DisplayPlane:' .. displayPlane)

	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.SimplifiedSymbols then
			featurePortrayal:AddInstructions('PointInstruction:BOYSAW12')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetX = -3.51
			if feature.buoyShape == 3 and feature.colour[1] == 1 and feature.colour[2] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPH50')
			elseif feature.buoyShape == 3 and feature.colour[1] == 3 and feature.colour[2] == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPH50')
			elseif feature.buoyShape == 4 and feature.colour[1] == 1 and feature.colour[2] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL50')
			elseif feature.buoyShape == 4 and feature.colour[1] == 3 and feature.colour[2] == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL50')
			elseif feature.buoyShape == 5 and feature.colour[1] == 1 and feature.colour[2] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR50')
			elseif feature.buoyShape == 5 and feature.colour[1] == 3 and feature.colour[2] == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR50')
			elseif feature.buoyShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPH01')
			elseif feature.buoyShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL01')
			elseif feature.buoyShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR01')
			elseif feature.buoyShape == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BOYBAR01')
			elseif feature.buoyShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSUP01')
				textOffsetX = -7.02
			elseif feature.buoyShape == 8 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR01')
			else
				featurePortrayal:AddInstructions('PointInstruction:BOYGEN03')
			end
			TOPMAR02(feature, featurePortrayal, contextParameters, viewingGroup, true)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:' .. textOffsetX .. ',3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
