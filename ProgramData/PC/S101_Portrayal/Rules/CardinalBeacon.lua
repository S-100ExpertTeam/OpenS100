-- #155
-- #238 [PSWG #117]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Beacon Cardinal main entry point.
function CardinalBeacon(feature, featurePortrayal, contextParameters)
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
			if feature.categoryOfCardinalMark == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNCAR04')
			elseif feature.categoryOfCardinalMark == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNCAR03')
			elseif feature.categoryOfCardinalMark == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNCAR02')
			elseif feature.categoryOfCardinalMark == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNCAR01')
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNDEF13')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetY = 3.51
			if feature.colour[1] == 2 and feature.colour[2] == 6 and feature.colour[3] == 2 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW23')
			elseif feature.colour[1] == 2 and feature.colour[2] == 6 and feature.colour[3] == 2 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN23')
			elseif feature.colour[1] == 2 and feature.colour[2] == 6 and feature.colour[3] == 2 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN23')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.colour[3] == 6 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW25')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.colour[3] == 6 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN25')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.colour[3] == 6 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN25')
			elseif feature.colour[1] == 2 and feature.colour[2] == 6 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW22')
			elseif feature.colour[1] == 2 and feature.colour[2] == 6 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN22')
			elseif feature.colour[1] == 2 and feature.colour[2] == 6 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN22')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW24')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN24')
			elseif feature.colour[1] == 6 and feature.colour[2] == 2 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN24')
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
			elseif feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
				textOffsetY = 7.02
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN03')
			end
			TOPMAR02(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,' .. textOffsetY .. ';TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), textViewingGroup, textPriority, viewingGroup, priority)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
