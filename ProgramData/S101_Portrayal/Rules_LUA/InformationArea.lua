-- Information area main entry point.
function InformationArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization

	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Curve then

	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Surface then

	featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR;NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
