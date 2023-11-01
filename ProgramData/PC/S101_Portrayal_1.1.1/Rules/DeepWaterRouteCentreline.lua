-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Deep Water Route Centreline main entry point.
function DeepWaterRouteCentreline(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.basedOnFixedMarks and feature.trafficFlow == 1 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL08')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 2 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL08')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 3 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL08')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 4 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL06')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 1 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 2 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 3 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 4 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL05')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.trafficFlow == 1 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.trafficFlow == 2 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.trafficFlow == 3 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL07')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		elseif feature.trafficFlow == 4 then
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWRTCL05')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		else
			viewingGroup = 25010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:DWLDEF01')
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25010, 18)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
