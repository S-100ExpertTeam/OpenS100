-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Water turbulence main entry point.
function WaterTurbulence(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 33040
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33040;DrawingPriority:9;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33040;DrawingPriority:9;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:WATTUR02')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 33040
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33040;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:WATTUR02')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 33040
		featurePortrayal:AddInstructions('ViewingGroup:33040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:WATTUR02')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
