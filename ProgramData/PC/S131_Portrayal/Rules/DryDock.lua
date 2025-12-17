-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Modified for S-131 1.0.0 by RMM
-- S-131 allows both point and surface geometry and uses a magenta outline and brown fill
-- TODO: enhance symbology for both point and surface DryDock features?
-- TODO: revisit when S-101 point symbol is developed

-- Dry dock main entry point.
function DryDock(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 32440
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:DRYDOC91')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32440
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:12;DisplayPlane:UnderRADAR')
--		featurePortrayal:AddInstructions('ColorFill:LANDA')
--		featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
    featurePortrayal:AddInstructions('ColorFill:LANDA')
    featurePortrayal:SimpleLineStyle('solid',0.32,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
