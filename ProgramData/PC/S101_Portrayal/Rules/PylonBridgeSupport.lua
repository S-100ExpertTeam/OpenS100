-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Pylon/bridge support main entry point.
function PylonBridgeSupport(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 12210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12210
		featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:18;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
