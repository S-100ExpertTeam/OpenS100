require 'S101AttributeSupport'
--
-- Issue #432
--

-- Gate main entry point.
function Gate(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfGate == 2 then
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		else
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfGate == 2 then
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		else
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfGate == 2 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		elseif feature.categoryOfGate == 5 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12410
		featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local featureName = GetFeatureName(feature, contextParameters)
	if featureName or HasHorizontalClearance(feature) then
		-- Center annotationas in or on the feature
		local xOffset = 0
		local yOffset = 0
		if feature.PrimitiveType == PrimitiveType.Point then
			-- Center annotations underneath the symbol
			featurePortrayal:AddInstructions('TextAlignVertical:Top')
			yOffset = -3.51
			featurePortrayal:AddInstructions('LocalOffset:'.. xOffset ..',' .. yOffset .. ';TextAlignHorizontal:Center;FontColor:CHBLK')
		elseif 	feature.PrimitiveType == PrimitiveType.Curve then
			-- to limit overwriting of vertical lines, place below (default alignments Horiz = Start, Vert = Bottom)
			xOffset = 3.51
			yOffset = -3.51
			featurePortrayal:AddInstructions('LocalOffset:'.. xOffset ..',' .. yOffset .. ';FontColor:CHBLK')
		else
			featurePortrayal:AddInstructions('TextAlignVertical:Center')
			featurePortrayal:AddInstructions('LocalOffset:'.. xOffset ..',' .. yOffset .. ';TextAlignHorizontal:Center;FontColor:CHBLK')
		end

		if featureName then
			featurePortrayal:AddTextInstruction(EncodeString(featureName), 26, 24, viewingGroup, 9)
			yOffset = yOffset - 3.51
		end
		PortrayClearances(feature, featurePortrayal, contextParameters, viewingGroup, 0, yOffset)
	end

	return viewingGroup
end
