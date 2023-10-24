-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Deep water route part main entry point.
function DeepWaterRoutePart(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:SimpleLineStyle('dash',0.96,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:SimpleLineStyle('dash',0.96,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:SimpleLineStyle('dash',0.96,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:DWRUTE51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:SimpleLineStyle('dash',0.96,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TSLDEF51')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:SimpleLineStyle('dash',0.96,'TRFCD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:AddInstructions('LineInstruction:DWRUTE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:AddInstructions('LineInstruction:DWRUTE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:AddInstructions('LineInstruction:DWRUTE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:DWRUTE51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:AddInstructions('LineInstruction:DWRUTE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 25010
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TSLDEF51')
			featurePortrayal:AddInstructions('PointInstruction:DWRTPT51')
			featurePortrayal:AddInstructions('LineInstruction:DWRUTE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
