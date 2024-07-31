
-- Magnetic variation main entry point.
function MagneticVariation(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 31080
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:MAGVAR01')
		featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(feature['valueOfMagneticVariation']), 27, 24, 31080, 12)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 31080
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.64,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:MAGVAR51')
		featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(feature.valueOfMagneticVariation, 'varn %3.2f'), 27, 24, 31080, 12)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 31080
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:MAGVAR51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
