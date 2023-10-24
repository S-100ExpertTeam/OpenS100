-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155
-- #133, [PSWG #111]

-- Radio station main entry point.
function RadioStation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if contains(10, feature.categoryOfRadioStation) then
			viewingGroup = 38010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DRFSTA02')
		else
			viewingGroup = 38010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contains(10, feature.categoryOfRadioStation) then
			viewingGroup = 38010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DRFSTA02')
		else
			viewingGroup = 38010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
