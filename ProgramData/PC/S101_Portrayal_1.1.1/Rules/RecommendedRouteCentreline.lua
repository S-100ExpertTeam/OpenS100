-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Manual update for FC 1.0.1 (categoryOfRecomendedTrack -> basedOnFixedMarks) (2022/4/25)

-- Recommended Route Centreline main entry point.
function RecommendedRouteCentreline(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.basedOnFixedMarks and feature.trafficFlow == 1 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL14')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 2 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL14')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 3 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL14')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.basedOnFixedMarks and feature.trafficFlow == 4 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL13')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 1 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 2 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 3 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif not feature.basedOnFixedMarks and feature.trafficFlow == 4 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL11')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.trafficFlow == 1 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.trafficFlow == 2 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.trafficFlow == 3 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL12')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		elseif feature.trafficFlow == 4 then
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRTCL11')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		else
			viewingGroup = 25020
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25020;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:RCRDEF11')
			if feature.orientationValue then
				featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25020, 18)
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
