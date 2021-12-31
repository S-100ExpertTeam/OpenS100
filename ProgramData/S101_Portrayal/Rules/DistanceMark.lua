-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Distance mark main entry point.
function DistanceMark(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedPoints then
		if feature.categoryOfDistanceMark == 1 then
			viewingGroup = 27030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DISMAR04')
		else
			viewingGroup = 27030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DISMAR03')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfDistanceMark == 1 then
			viewingGroup = 27030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DISMAR04')
		else
			viewingGroup = 27030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DISMAR03')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
