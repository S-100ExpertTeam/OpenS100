-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Canal main entry point.
function Canal(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12420
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.condition then
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.condition then
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
