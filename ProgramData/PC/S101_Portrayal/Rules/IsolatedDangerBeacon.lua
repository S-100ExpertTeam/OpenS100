-- #155
-- #238 [PSWG #117]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Beacon Isolated Danger main entry point.
function IsolatedDangerBeacon(feature, featurePortrayal, contextParameters)
	local viewingGroup = 27020
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
			featurePortrayal:AddInstructions('PointInstruction:BCNISD21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetY = 7.02
			if feature.beaconShape == 3 and feature.colour[1] == 2 and feature.colour[2] == 3 and feature.colour[3] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW40')
			elseif feature.beaconShape == 5 and feature.colour[1] == 2 and feature.colour[2] == 3 and feature.colour[3] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN40')
			elseif feature.beaconShape == 7 and feature.colour[1] == 2 and feature.colour[2] == 3 and feature.colour[3] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN40')
			elseif feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSTK02')
				localOffsetY = 3.51
			elseif feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW01')
			elseif feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLTC01')
			elseif feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			elseif feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN03')
				localOffsetY = 3.51
			end
			TOPMAR02(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,' .. textOffsetY .. ';TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
