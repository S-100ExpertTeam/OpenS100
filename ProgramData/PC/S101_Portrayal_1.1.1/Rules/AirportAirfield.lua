-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #134

-- Airport/airfield main entry point.
function AirportAirfield(feature, featurePortrayal, contextParameters)
	local viewingGroup
	local categoryOfAirportAirfield = feature.categoryOfAirportAirfield

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32240
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		if categoryOfAirportAirfield == 3 or categoryOfAirportAirfield == 4 then
		    featurePortrayal:AddInstructions('PointInstruction:HELIPD02')
		else
		    featurePortrayal:AddInstructions('PointInstruction:AIRARE02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32240
		featurePortrayal:AddInstructions('ViewingGroup:32240;DrawingPriority:6;DisplayPlane:UnderRADAR')
		if categoryOfAirportAirfield == 3 or categoryOfAirportAirfield == 4 then
			featurePortrayal:AddInstructions('PointInstruction:HELIPD02')
		else
			featurePortrayal:AddInstructions('AreaFillReference:AIRARE02')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
