-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Recommended track main entry point.
function RecommendedTrack(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 1 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 2 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 3 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 4 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC10')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 1 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 2 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 3 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 4 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC09')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.trafficFlow == 1 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.trafficFlow == 2 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.trafficFlow == 3 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		elseif feature.trafficFlow == 4 then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC09')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECDEF02')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,24')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
