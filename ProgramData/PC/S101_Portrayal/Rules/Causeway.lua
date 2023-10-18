-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Causeway main entry point.
function Causeway(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.waterLevelEffect == 4 then
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.96,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 22010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.waterLevelEffect == 4 then
			viewingGroup = 22010
			featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPIT')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 22010
			featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.waterLevelEffect == 4 then
			viewingGroup = 22010
			featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPIT')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 22010
			featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
