-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Recommended track main entry point.
function RecommendedTrack(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC12')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC10')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC09')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.trafficFlow == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.trafficFlow == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.trafficFlow == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC11')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		elseif feature.trafficFlow == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECTRC09')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RECDEF02')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC56')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC55')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC55')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RECDEF51')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0fdeg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC58')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 1 and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC56')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.categoryOfRecommendedTrack == 2 and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC55')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.trafficFlow == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.trafficFlow == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.trafficFlow == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC57')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.orientationValue and feature.trafficFlow == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:RECTRC55')
			featurePortrayal:AddInstructions('LocalOffset:14.04,0;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(feature.orientationValue, '%03.0f deg') .. ',11,8')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RECDEF51')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
