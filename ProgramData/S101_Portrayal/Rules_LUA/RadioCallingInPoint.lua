-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Radio calling-in point main entry point.
function RadioCallingInPoint(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:')
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s') .. ',21,8')
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
