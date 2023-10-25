-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Fairway main entry point.
function Fairway(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue and feature.trafficFlow == 1 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:FAIRWY52')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 26050
			featurePortrayal:AddInstructions('ViewingGroup:26050;DrawingPriority:12;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26050, 12)
			end
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
