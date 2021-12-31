-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Canal main entry point.
function Canal(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.condition then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.condition then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:2;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
