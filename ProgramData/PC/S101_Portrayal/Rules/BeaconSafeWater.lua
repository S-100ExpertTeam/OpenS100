-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155

-- Referenced portrayal rules.
require 'TOPMAR01'

-- Beacon Safe Water main entry point.
function BeaconSafeWater(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard;Hover:true')

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.beaconShape == 1 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 2 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 7 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 3 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif contains(11, feature.natureOfConstruction) then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 5 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		else
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSAW13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.beaconShape == 1 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSTK02')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 3 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNTOW01')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif contains(11, feature.natureOfConstruction) then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNLTC01')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 5 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		elseif feature.beaconShape == 7 then
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		else
			viewingGroup = 27020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN03')
			TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, false)
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'bn %s'), 21, 24, 27020, 24)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
