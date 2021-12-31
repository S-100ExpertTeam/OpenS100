-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Ferry route main entry point.
function FerryRoute(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfFerry[1] == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT01')
		elseif feature.categoryOfFerry[1] == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT02')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FERYRT02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfFerry[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE52')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfFerry[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE52')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FRYARE51')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
