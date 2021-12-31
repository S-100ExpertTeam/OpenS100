-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Lake main entry point.
function Lake(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:2;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:DEPVS')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
