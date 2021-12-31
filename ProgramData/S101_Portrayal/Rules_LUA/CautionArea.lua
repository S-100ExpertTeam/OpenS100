-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Caution Area main entry point.
function CautionArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO06')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:3;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:3;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTNARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
