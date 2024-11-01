-- #155
-- #238 [PSWG #117]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Buoy Lateral main entry point.
function LateralBuoy(feature, featurePortrayal, contextParameters)
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
			if feature.buoyShape == 1 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT14')
			elseif feature.buoyShape == 1 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT13')
			elseif feature.buoyShape == 2 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT24')
			elseif feature.buoyShape == 2 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT23')
			elseif feature.categoryOfLateralMark == 3 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT24')
			elseif feature.categoryOfLateralMark == 3 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT23')
			elseif feature.categoryOfLateralMark == 4 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT14')
			elseif feature.categoryOfLateralMark == 4 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT13')
			elseif feature.buoyShape == 1 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT14')
			elseif feature.buoyShape == 1 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT13')
			elseif feature.buoyShape == 2 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT24')
			elseif feature.buoyShape == 2 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT23')
			elseif feature.categoryOfLateralMark == 1 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT24')
			elseif feature.categoryOfLateralMark == 1 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT23')
			elseif feature.categoryOfLateralMark == 2 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT14')
			elseif feature.categoryOfLateralMark == 2 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYLAT13')
			else
				featurePortrayal:AddInstructions('PointInstruction:BOYDEF03')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetX = -3.51
			-- Red,Green,Red
			if feature.buoyShape == 1 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCON30')
			elseif feature.buoyShape == 2 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAN30')
			elseif feature.buoyShape == 4 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL30')
			elseif feature.buoyShape == 5 and feature.colour[1] == 3 and feature.colour[2] == 4 and feature.colour[3] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR30')
			-- Green,Red,Green
			elseif feature.buoyShape == 1 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCON31')
			elseif feature.buoyShape == 2 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAN31')
			elseif feature.buoyShape == 4 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL31')
			elseif feature.buoyShape == 5 and feature.colour[1] == 4 and feature.colour[2] == 3 and feature.colour[3] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR31')
			elseif feature.buoyShape == 1 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCON10')
			elseif feature.buoyShape == 1 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCON11')
			elseif feature.buoyShape == 2 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAN10')
			elseif feature.buoyShape == 2 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAN11')
			elseif feature.buoyShape == 3 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPH10')
			elseif feature.buoyShape == 3 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPH11')
			elseif feature.buoyShape == 4 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL10')
			elseif feature.buoyShape == 4 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL11')
			elseif feature.buoyShape == 5 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR10')
			elseif feature.buoyShape == 5 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR11')
			elseif feature.buoyShape == 6 and feature.colour[1] == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYBAR10')
			elseif feature.buoyShape == 6 and feature.colour[1] == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYBAR11')
			elseif feature.buoyShape == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCON01')
			elseif feature.buoyShape == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAN01')
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
