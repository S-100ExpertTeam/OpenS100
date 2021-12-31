-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Waterfall main entry point.
function Waterfall(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.visuallyConspicuous == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:3;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:3;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHWHT')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:3;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32050;DrawingPriority:3;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRF')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
