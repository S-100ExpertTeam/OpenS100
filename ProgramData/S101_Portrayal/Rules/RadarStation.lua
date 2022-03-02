-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Radar station main entry point.
function RadarStation(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedPoints then
		if contains(2, feature.categoryOfRadarStation) then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:0,0;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,24')
			end
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contains(2, feature.categoryOfRadarStation) then
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
			if feature.communicationChannel[1] then
				featurePortrayal:AddInstructions('LocalOffset:0,0;FontSize:10;TextInstruction:' .. EncodeString(feature.communicationChannel[1], 'ch %s') .. ',11,24')
			end
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
