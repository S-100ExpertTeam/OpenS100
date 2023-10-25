-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence

-- Waterfall main entry point.
function Waterfall(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32050
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRADAR;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR;NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.visualProminence == 1 then
			viewingGroup = 32050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHWHT')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32050
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:9;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
