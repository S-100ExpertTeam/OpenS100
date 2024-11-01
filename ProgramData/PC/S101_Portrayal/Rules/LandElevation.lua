
-- Land elevation main entry point.
function LandElevation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:POSGEN04')
		featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(feature.elevation, '%3.0f m'), 28, 24, 32010, 12)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
