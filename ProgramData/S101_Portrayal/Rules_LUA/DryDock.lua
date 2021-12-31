-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Dry dock main entry point.
function DryDock(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:LANDA')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
