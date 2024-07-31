-- #155

-- Recommended traffic lane part main entry point.
function RecommendedTrafficLanePart(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.orientationValue then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RCTLPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:RTLDEF51')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.orientationValue then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RCTLPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:RTLDEF51')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.orientationValue then
			viewingGroup = 25020
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RCTLPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 25020
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:RTLDEF51')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue then
			viewingGroup = 25020
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RCTLPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
		else
			viewingGroup = 25020
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:RTLDEF51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
