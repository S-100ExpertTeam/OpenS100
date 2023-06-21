-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Vegetation main entry point.
function Vegetation(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedPoints then
		if feature.categoryOfVegetation == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 21 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfVegetation == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		elseif feature.categoryOfVegetation == 21 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT05')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TREPNT04')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfVegetation == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfVegetation == 21 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN03')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfVegetation == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfVegetation == 21 then
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:VEGATN03')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
