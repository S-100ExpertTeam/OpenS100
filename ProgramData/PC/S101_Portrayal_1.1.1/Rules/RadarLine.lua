-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Radar line main entry point.
function RadarLine(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 25040
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25040;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25040;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;FontSize:10')
		featurePortrayal:AddTextInstruction(EncodeString(feature.orientationValue, '%03.0f deg'), 11, 24, 25040, 18)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
