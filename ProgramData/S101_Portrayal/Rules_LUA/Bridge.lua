-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Bridge main entry point.
function Bridge(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfBridge == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		elseif feature.categoryOfBridge == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',21,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfBridge == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',21,8')
			end
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfBridge == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfBridge == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',21,8')
			end
			featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.None then
		featurePortrayal:AddInstructions('NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
