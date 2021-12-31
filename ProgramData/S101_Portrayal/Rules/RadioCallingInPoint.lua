-- Radio calling-in point main entry point.
function RadioCallingInPoint(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedPoints then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.trafficFlow == 1 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 2 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 3 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL02')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		elseif feature.trafficFlow == 4 and feature.orientationValue[1] then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue[1]) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RDOCAL03')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.communicationChannel[1], 'ch %s'), 11, 24, 25060, 18)
			end
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25060;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:RCLDEF01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 21, 24, 25060, 18)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 25060
end
