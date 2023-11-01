-- VesselTrafficServiceArea portrayal rules file.
--
-- #93 task 1: Update to support surfaces instead of points
-- Issues: PSWG #50, PC #117
--

-- Main entry point for feature type.
function VesselTrafficServiceArea(feature, featurePortrayal, contextParameters)
	local viewingGroup = 36050

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:VTSARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	-- provide centered text for name regardless of boundary
	if feature.PrimitiveType == PrimitiveType.Surface and feature.featureName[1] and feature.featureName[1].name then
		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHMGD')
		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'VTS %s'), 21, 24, 36050, 18)
	end

	return viewingGroup
end
