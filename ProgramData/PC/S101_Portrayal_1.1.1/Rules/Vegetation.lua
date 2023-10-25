-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155
-- PSWG #109, PC #131 

-- Vegetation main entry point.
function Vegetation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfVegetation == 7 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 21 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 11 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:MARSHE01')
		else
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfVegetation == 7 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 21 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 11 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:MARSHE01')
		else
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32030
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfVegetation == 7 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfVegetation == 21 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN03')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfVegetation == 7 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfVegetation == 21 then
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32030
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN03')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
