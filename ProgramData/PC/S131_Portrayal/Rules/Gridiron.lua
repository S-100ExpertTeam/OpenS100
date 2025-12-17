-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Modified for S-131 1.0.0 by RMM
-- S-131 also allows point primitives

-- Gridiron main entry point.
function Gridiron(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 27060
		featurePortrayal:AddInstructions('ViewingGroup:27060;DrawingPriority:15;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
  elseif feature.PrimitiveType == PrimitiveType.Point then
  	featurePortrayal:AddInstructions('PointInstruction:GRIDRN91')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
