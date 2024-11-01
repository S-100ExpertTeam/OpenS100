-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence

-- Waterfall main entry point.
function Waterfall(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar')
		end
		if feature.visualProminence and feature.visualProminence == 1 then
			-- visually conspicuous
			featurePortrayal:AddInstructions('PointInstruction:WATFAL01')
		else
			featurePortrayal:AddInstructions('PointInstruction:WATFAL02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.visualProminence == 1 then
			viewingGroup = 32050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHWHT')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRadar')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
