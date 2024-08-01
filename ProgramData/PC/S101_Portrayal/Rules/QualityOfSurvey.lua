
-- Quality of Survey main entry point.
function QualityOfSurvey(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 31010
		featurePortrayal:AddInstructions('ViewingGroup:31010;DrawingPriority:0;DisplayPlane:UnderRadar;NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 31010
		featurePortrayal:AddInstructions('ViewingGroup:31010;DrawingPriority:0;DisplayPlane:UnderRadar;NullInstruction')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
