-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Vertical datum of data main entry point.
function VerticalDatumOfData(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 31010
		featurePortrayal:AddInstructions('ViewingGroup:31010;DrawingPriority:0;DisplayPlane:UnderRADAR;NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
