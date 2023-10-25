-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Bridge main entry point.
function Bridge(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfBridge == 2 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 3 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 4 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 5 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 7 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 8 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		else
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12210, 24)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfBridge == 2 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 3 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 4 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 5 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 7 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 8 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12210, 24)
			end
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfBridge == 2 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 3 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 4 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 5 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 7 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfBridge == 8 then
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12210, 24)
			end
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.None then
		viewingGroup = 12210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR;NullInstruction')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
