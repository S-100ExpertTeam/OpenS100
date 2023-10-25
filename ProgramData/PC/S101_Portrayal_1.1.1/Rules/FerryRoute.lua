-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Ferry route main entry point.
function FerryRoute(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if contains(1, feature.categoryOfFerry) or contains(5, feature.categoryOfFerry) then
			viewingGroup = 25030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT01')
		elseif contains(2, feature.categoryOfFerry) then
			viewingGroup = 25030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT02')
		else
			viewingGroup = 25030
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(2, feature.categoryOfFerry) then
			viewingGroup = 25030
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE52')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 25030
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(2, feature.categoryOfFerry) then
			viewingGroup = 25030
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE52')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		else
			viewingGroup = 25030
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE51')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
