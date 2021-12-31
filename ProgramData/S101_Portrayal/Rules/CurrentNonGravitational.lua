-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Current - non-gravitational main entry point.
function CurrentNonGravitational(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedPoints then
		if feature.orientation and feature.orientation.orientationValue and feature.speed and feature.speed.speedMaximum then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 15)
		elseif feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.orientation and feature.orientation.orientationValue and feature.speed and feature.speed.speedMaximum then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.speed.speedMaximum, '%4.1f kn'), 30, 24, 33060, 15)
		elseif feature.orientation and feature.orientation.orientationValue then
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientation.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:CURENT01')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 33060
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:33060;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CURDEF01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		--TODO: Determine S-101 portrayal for CurrentNonGravitational curve feature.
		viewingGroup = 21010
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;PointInstruction:testPCB')
		Debug.Trace('Warning: S-52 does not define portrayal for CurrentNonGravitational curve features.')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		--TODO: Determine S-101 portrayal for CurrentNonGravitational surface feature.
		viewingGroup = 21010
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;PointInstruction:testPCB')
		Debug.Trace('Warning: S-52 does not define portrayal for CurrentNonGravitational surface features.')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
