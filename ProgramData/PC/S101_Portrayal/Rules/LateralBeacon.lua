-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155
-- #238 [PSWG #117]
-- #356 [PSWG #153]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Beacon Lateral main entry point.
function LateralBeacon(feature, featurePortrayal, contextParameters)
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
			if feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 3 and feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 3 and feature.beaconShape == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 3 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT21')
			elseif feature.colour[1] == 4 and feature.beaconShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 4 and feature.beaconShape == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 4 and feature.beaconShape == 7 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT22')
			elseif feature.colour[1] == 3 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 3 and feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 3 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 4 and feature.beaconShape == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 4 and feature.beaconShape == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.colour[1] == 4 and feature.beaconShape == 5 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			elseif feature.beaconShape == 6 and feature.visualProminence == 1 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
			elseif feature.beaconShape == 6 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
			elseif feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT15')
			elseif feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNLAT16')
			else
				featurePortrayal:AddInstructions('PointInstruction:BCNDEF13')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetY = 3.51
			-- Red,Green,Red
			if feature.beaconShape == 3 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW30')
			elseif feature.beaconShape == 5 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN30')
			elseif feature.beaconShape == 7 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN30')
			-- Green,Red,Green
			elseif feature.beaconShape == 3 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW31')
			elseif feature.beaconShape == 5 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN31')
			elseif feature.beaconShape == 7 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN31')
			-- Red
			elseif feature.beaconShape == 3 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW10')
			elseif feature.beaconShape == 5 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN10')
			elseif feature.beaconShape == 7 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN10')
			-- Green
			elseif feature.beaconShape == 3 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNTOW11')
			elseif feature.beaconShape == 5 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN11')
			elseif feature.beaconShape == 7 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BCNGEN11')
			elseif feature.beaconShape == 2 and feature.categoryOfLateralMark == 1 then
				featurePortrayal:AddInstructions('PointInstruction:PRICKE03')
			elseif feature.beaconShape == 2 and feature.categoryOfLateralMark == 2 then
				featurePortrayal:AddInstructions('PointInstruction:PRICKE04')
			elseif feature.beaconShape == 6 and feature.visualProminence == 1 then
				featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
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
				featurePortrayal:AddInstructions('LocalOffset:-3.51,' .. textOffsetY .. ';TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
