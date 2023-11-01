-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #176
-- #134

-- Runway main entry point.
function Runway(feature, featurePortrayal, contextParameters)
	local viewingGroup
	local categoryOfRunway = feature.categoryOfRunway
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		if categoryOfRunway == 1 then
			featurePortrayal:AddInstructions('PointInstruction:AIRARE02')
		elseif categoryOfRunway == 2 then
			featurePortrayal:AddInstructions('PointInstruction:HELIPD02')
		else
			featurePortrayal:AddInstructions('NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32240
		featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:15;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
