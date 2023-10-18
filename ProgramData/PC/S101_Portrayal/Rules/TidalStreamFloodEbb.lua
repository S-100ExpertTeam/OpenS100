-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155

-- Tidal stream - flood/ebb main entry point.
function TidalStreamFloodEbb(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfTidalStream == 1 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FLDSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 2 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:EBBSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 3 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		else
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfTidalStream == 1 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FLDSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 2 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:EBBSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 3 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		else
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfTidalStream == 1 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FLDSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 2 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:EBBSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 3 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		else
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfTidalStream == 1 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FLDSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 2 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:EBBSTR01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		elseif feature.categoryOfTidalStream == 3 and feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 12)
		else
			viewingGroup = 33060
			featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
			featurePortrayal:AddInstructions('LineInstruction:TIDINF51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
