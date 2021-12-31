-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Radio station main entry point.
function RadioStation(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.categoryOfRadioStation == 10 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DRFSTA01')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfRadioStation == 10 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DRFSTA01')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
