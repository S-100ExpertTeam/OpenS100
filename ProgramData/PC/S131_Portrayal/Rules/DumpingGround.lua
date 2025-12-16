-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- adapted for S131 by RMM, removed call to RESTRN01 because S-131 1.0.0 does not have restrictions

-- Dumping ground main entry point.
function DumpingGround(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26240;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26240;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26240
		featurePortrayal:AddInstructions('ViewingGroup:26240;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26240
		featurePortrayal:AddInstructions('ViewingGroup:26240;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTYARE51')

	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
