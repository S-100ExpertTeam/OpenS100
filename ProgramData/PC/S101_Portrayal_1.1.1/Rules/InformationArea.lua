-- Information area main entry point.
-- #110
-- #167

function InformationArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:31020;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:31020;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:INFARE01')
	--#167: curve geometry removed in FC 1.1.0
	--elseif feature.PrimitiveType == PrimitiveType.Curve then
		--featurePortrayal:AddInstructions('ViewingGroup:31020;DrawingPriority:12;DisplayPlane:UnderRADAR')
		--featurePortrayal:AddInstructions('LineInstruction:INFARE51')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:31020;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
		featurePortrayal:AddInstructions('Dash:2,6;LineStyle:_simple_,8,0.32,CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 31020
end
