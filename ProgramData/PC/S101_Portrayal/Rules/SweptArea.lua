-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Swept Area main entry point.
function SweptArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 23030
		featurePortrayal:AddInstructions('ViewingGroup:23030;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:SWPARE51')
		featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
		featurePortrayal:AddTextInstruction(EncodeString(feature.depthRangeMinimumValue, 'swept to %5.1f'), 27, 24, 23030, 12)
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 23030
		featurePortrayal:AddInstructions('ViewingGroup:23030;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:SWPARE51')
		featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
		featurePortrayal:AddTextInstruction(EncodeString(feature.depthRangeMinimumValue, 'swept to %5.1f'), 27, 24, 23030, 12)
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
