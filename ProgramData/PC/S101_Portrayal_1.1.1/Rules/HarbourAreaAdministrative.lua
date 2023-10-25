-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Harbour area (administrative) main entry point.
function HarbourAreaAdministrative(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 36020
		featurePortrayal:AddInstructions('ViewingGroup:36020;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 36020
		featurePortrayal:AddInstructions('ViewingGroup:36020;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
