-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Gridiron main entry point.
function Gridiron(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 27060
		featurePortrayal:AddInstructions('ViewingGroup:27060;DrawingPriority:15;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
