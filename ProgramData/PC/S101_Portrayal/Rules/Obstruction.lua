--
-- #48
-- #89
-- #103, PSWG #71
-- #23, PSWG #34
-- #155
-- #168

--
-- Referenced portrayal rules.
require 'OBSTRN07'

-- Obstruction main entry point.
function Obstruction(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfObstruction == 8 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 8 then
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 then
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 then 
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 21 then		-- active submarine volcano
			viewingGroup = 14020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:14020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:14020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('AlertReference:NavHazard;PointInstruction:VOLCANO01')
		elseif feature.categoryOfObstruction == 23 then		-- mangrove
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:VEGATN04P')
		elseif feature.waterLevelEffect == 7 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('AlertReference:NavHazard;PointInstruction:FLTHAZ02')
		else
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfObstruction == 8 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 and feature.valueOfSounding then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 8 then
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 then
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 then
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 21 then			-- active submarine volcano
			viewingGroup = 14020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:14020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:14020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('AlertReference:NavHazard;PointInstruction:VOLCANO01')
		elseif feature.categoryOfObstruction == 23 then			-- mangrove
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:VEGATN04P')
		elseif feature.waterLevelEffect == 7 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('AlertReference:NavHazard;PointInstruction:FLTHAZ02')
		else
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfObstruction == 8 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 9 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 10 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 21 then
			--#168
			--viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
			error('Curve is not a valid geometry for active submarine volcano')
		elseif feature.categoryOfObstruction == 23 then
			--#313
			error('Curve is not a valid geometry for mangrove')
		elseif feature.waterLevelEffect == 7 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('AlertReference:NavHazard;LineInstruction:_simple_')
		else
			viewingGroup = 34050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfObstruction == 8 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 9 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 10 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 21 then
			--#168
			error('Surface is not a valid geometry for active submarine volcano')
		elseif feature.categoryOfObstruction == 23 then  -- mangrove
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_;LinePlacement:Relative,0.5')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')			
		elseif feature.waterLevelEffect == 7 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('AlertReference:NavHazard;LineInstruction:_simple_')
		else
			viewingGroup = 34050
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfObstruction == 8 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 9 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 10 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfObstruction == 21 then	-- volcano
			--#168
			--viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
			error('Surface is not a valid geometry for active submarine volcano')
		elseif feature.categoryOfObstruction == 23 then  -- mangrove
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_;LinePlacement:Relative,0.5')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')			
		elseif feature.waterLevelEffect == 7 then
			viewingGroup = 12410
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('AlertReference:NavHazard;LineInstruction:_simple_')
		else
			viewingGroup = 34050
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:12;DisplayPlane:UnderRadar')
			viewingGroup = OBSTRN07(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	return viewingGroup
end
