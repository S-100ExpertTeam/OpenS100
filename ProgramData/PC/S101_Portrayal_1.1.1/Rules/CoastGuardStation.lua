-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Manually updated for FC 1.0.2 (2022/4/22)
--
-- Issues: PC #122

-- Coastguard station main entry point.
function CoastGuardStation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	viewingGroup = 38030

	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:21;DisplayPlane:OverRADAR')
	else
		featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:21;DisplayPlane:UnderRADAR')
	end
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('PointInstruction:CGUSTA02')

	elseif feature.PrimitiveType == PrimitiveType.Surface then
		--PC #122 
		featurePortrayal:AddInstructions('ViewingGroup:38030;DrawingPriority:21;PointInstruction:CGUSTA02')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
