-- #172

function SeparationZoneOrLine(feature, featurePortrayal, contextParameters)
	local viewingGroup = 25010

	if feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',1.92,'TRFCF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- S-52 PresLib 4.0.3 Part 1 clause 10.5.10 TSEZNE AREA
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53012,53012')

		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:TRFCF,0.75')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
