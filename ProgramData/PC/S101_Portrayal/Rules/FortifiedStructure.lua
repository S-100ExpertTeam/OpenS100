-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155
-- #256

-- Fortified structure main entry point.
function FortifiedStructure(feature, featurePortrayal, contextParameters)
	local viewingGroup = 32220
	
	if feature.visualProminence == 1 then
		viewingGroup = 22220
	end
	
	-- #256
	if feature.inTheWater then
		viewingGroup = 12200
	end
	
	if feature.PrimitiveType == PrimitiveType.Point or feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
	end
	
	-- Paper chart and simplified symbols are the same
	if feature.PrimitiveType == PrimitiveType.Point then
		if feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:FORSTC11')
		else
			featurePortrayal:AddInstructions('PointInstruction:FORSTC01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:SimpleLineStyle('solid',0.96,'LANDF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries are the same
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		if feature.visualProminence == 1 then
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		else
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
		end
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
