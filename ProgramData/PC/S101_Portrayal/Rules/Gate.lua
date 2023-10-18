-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155

-- Gate main entry point.
function Gate(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfGate == 2 then
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		else
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfGate == 2 then
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 32440
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		else
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfGate == 2 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfGate == 3 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:GATCON04')
		elseif feature.categoryOfGate == 4 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:GATCON03')
		elseif feature.categoryOfGate == 5 then
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12410
		featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
