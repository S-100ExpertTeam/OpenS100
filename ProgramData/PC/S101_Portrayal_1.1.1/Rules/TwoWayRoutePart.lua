-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Two-way route part main entry point.
function TwoWayRoutePart(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TWRDEF51')
			featurePortrayal:SimpleLineStyle('dash',1.28,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT53')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TWRTPT52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		else
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TWRDEF51')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
