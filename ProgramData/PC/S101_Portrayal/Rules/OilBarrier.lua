-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Oil barrier main entry point.
function OilBarrier(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
