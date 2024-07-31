-- #155
-- #238 [PSWG #117]

-- Referenced portrayal rules.
require 'TOPMAR02'

-- Buoy Cardinal main entry point.
function CardinalBuoy(feature, featurePortrayal, contextParameters)
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
			if feature.categoryOfCardinalMark == 4 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAR04')
			elseif feature.categoryOfCardinalMark == 3 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAR03')
			elseif feature.categoryOfCardinalMark == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAR02')
			elseif feature.categoryOfCardinalMark == 1 then
				featurePortrayal:AddInstructions('PointInstruction:BOYCAR01')
			else
				featurePortrayal:AddInstructions('PointInstruction:BOYDEF03')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), textViewingGroup, textPriority, viewingGroup, priority)
			end
		else
			local textOffsetX = -3.51
			
			-- Black,Yellow,Black
			if feature.buoyShape == 4 and feature.colour[1] == 2 and feature.colour[2] == 6 and feature.colour[3] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL23')
			elseif feature.buoyShape == 5 and feature.colour[1] == 2 and feature.colour[2] == 6 and feature.colour[3] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR23')
			-- Yellow,Black,Yellow
			elseif feature.buoyShape == 4 and feature.colour[1] == 6 and feature.colour[2] == 2 and feature.colour[3] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL25')
			elseif feature.buoyShape == 5 and feature.colour[1] == 6 and feature.colour[2] == 2 and feature.colour[3] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR25')
			-- Black,Yellow
			elseif feature.buoyShape == 4 and feature.colour[1] == 2 and feature.colour[2] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL22')
			elseif feature.buoyShape == 5 and feature.colour[1] == 2 and feature.colour[2] == 6 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR22')
			-- Yellow,Black
			elseif feature.buoyShape == 4 and feature.colour[1] == 6 and feature.colour[2] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYPIL24')
			elseif feature.buoyShape == 5 and feature.colour[1] == 6 and feature.colour[2] == 2 then
				featurePortrayal:AddInstructions('PointInstruction:BOYSPR24')
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
