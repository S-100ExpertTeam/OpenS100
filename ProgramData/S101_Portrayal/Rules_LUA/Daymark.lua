-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Daymark main entry point.
function Daymark(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.topmarkDaymarkShape == 19 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 20 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 21 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 24 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYTRI01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 25 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYTRI05')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.topmarkDaymarkShape == 19 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 20 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 21 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 24 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYTRI21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		elseif feature.topmarkDaymarkShape == 25 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYTRI25')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27025;DrawingPriority:7;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DAYSQR21')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'bn %s') .. ',21,8')
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
