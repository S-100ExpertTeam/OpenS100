-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155
-- #256

-- Building main entry point.
function Building(feature, featurePortrayal, contextParameters)
	local viewingGroup = 22220
	local drawingPriority = 18
	
	if feature.inTheWater then
		-- Dangers above water
		viewingGroup = 12200
	elseif feature.visualProminence == 1 then
		-- Visually conspicuous object
		viewingGroup = 22220
		if feature.PrimitiveType == PrimitiveType.Surface then
			drawingPriority = 12
		end
	else
		viewingGroup = 32220
		drawingPriority = 12
	end
	featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup)
	featurePortrayal:AddInstructions('DrawingPriority:' .. drawingPriority)
	
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('DisplayPlane:OverRADAR')
	else
		featurePortrayal:AddInstructions('DisplayPlane:UnderRADAR')
	end

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if contains(33, feature['function']) and feature.visualProminence == 1 and feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
		elseif contains(20, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(21, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(22, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(23, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(24, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(25, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(26, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(27, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(33, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(35, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif contains(33, feature['function']) and feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
		elseif feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUISGL11')
		elseif contains(20, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(21, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(22, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(23, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(24, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(25, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(26, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL05')
		elseif contains(27, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL05')
		elseif contains(33, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(35, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		else
			featurePortrayal:AddInstructions('PointInstruction:BUISGL01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contains(33, feature['function']) and feature.visualProminence == 1 and feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
		elseif contains(20, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(21, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(22, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(23, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(24, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(25, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL14')
		elseif contains(26, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(27, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(33, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(35, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif contains(33, feature['function']) and feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
		elseif feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:BUISGL11')
		elseif contains(20, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(21, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(22, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(23, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(24, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(25, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL04')
		elseif contains(26, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL05')
		elseif contains(27, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:BUIREL05')
		elseif contains(33, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(35, feature['function']) then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		else
			featurePortrayal:AddInstructions('PointInstruction:BUISGL01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(33, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(33, feature['function']) then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(33, feature['function']) and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(33, feature['function']) then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, drawingPriority)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
