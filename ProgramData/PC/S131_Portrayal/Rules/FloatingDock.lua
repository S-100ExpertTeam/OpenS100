-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Modified for S-131 1.0.0 by RMM

-- Floating dock main entry point.
function FloatingDock(feature, featurePortrayal, contextParameters)
	local viewingGroup

--	featurePortrayal:AddInstructions('AlertReference:SafetyContour,101,101')

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12410
		featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:6;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHMGF')
		featurePortrayal:SimpleLineStyle('solid',0.64,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Point then
		featurePortrayal:AddInstructions('PointInstruction:FLODOC91')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
