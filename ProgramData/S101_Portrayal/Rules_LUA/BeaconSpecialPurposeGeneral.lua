-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'TOPMAR01'

-- Beacon Special Purpose/General main entry point.
function BeaconSpecialPurposeGeneral(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.beaconShape == 6 and feature.visuallyConspicuous == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.categoryOfSpecialPurposeMark[1] == 18 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:NOTBRD11')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.categoryOfSpecialPurposeMark[1] == 44 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP13')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 6 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.beaconShape == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSPP21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.beaconShape == 6 and feature.visuallyConspicuous == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.categoryOfSpecialPurposeMark[1] == 18 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:NOTBRD11')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.categoryOfSpecialPurposeMark[1] == 44 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNSTK02')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNTOW01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNLTC01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 6 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		elseif feature.beaconShape == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27020;DrawingPriority:8;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BCNGEN03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
			TOPMAR01(feature, featurePortrayal, contextParameters, false)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
