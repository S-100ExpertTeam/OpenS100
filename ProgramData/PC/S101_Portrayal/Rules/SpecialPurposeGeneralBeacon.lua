-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155
-- #238 [PSWG #117]
-- #356 [PSWG #153]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Beacon Special Purpose/General main entry point.
function SpecialPurposeGeneralBeacon(feature, featurePortrayal, contextParameters)
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
			if feature.beaconShape == 6 and feature.visualProminence == 1 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
			elseif contains(18, feature.categoryOfSpecialPurposeMark) then
				featurePortrayal:AddInstructions('PointInstruction:NOTBRD11')
			elseif contains(44, feature.categoryOfSpecialPurposeMark) then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			elseif feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			elseif feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			elseif feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			elseif feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			elseif feature.beaconShape == 6 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
			elseif feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetY = 3.51
			if feature.beaconShape == 6 and feature.visualProminence == 1 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
			elseif contains(18, feature.categoryOfSpecialPurposeMark) then
				featurePortrayal:AddInstructions('PointInstruction:NOTBRD11')
			elseif contains(44, feature.categoryOfSpecialPurposeMark) then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
				textOffsetY = 7.02
			elseif feature.beaconShape == 3 and feature.colour[1] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW13')
			elseif feature.beaconShape == 3 and feature.colour[1] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW12')
			elseif feature.beaconShape == 5 and feature.colour[1] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN13')
			elseif feature.beaconShape == 5 and feature.colour[1] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN12')
			elseif feature.beaconShape == 7 and feature.colour[1] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN13')
			elseif feature.beaconShape == 7 and feature.colour[1] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN12')
			elseif feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNSTK02')
			elseif feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW01')
				textOffsetY = 7.02
			elseif feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLTC01')
				textOffsetY = 7.02
			elseif feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
				textOffsetY = 7.02
			elseif feature.beaconShape == 6 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
			elseif feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
				textOffsetY = 7.02
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN03')
			end
			TOPMAR02(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
