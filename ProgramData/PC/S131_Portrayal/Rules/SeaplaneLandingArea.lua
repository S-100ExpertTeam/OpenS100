-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- adapted for S131 by RMM
-- removed restrictions and alerting logic, s-131 1.0.0 does not encode restrictions or use alert catalog


-- Seaplane Landing Area main entry point.
function SeaplaneLandingArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26040
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO06')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26040
		featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26040
		featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTYARE51')

	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
