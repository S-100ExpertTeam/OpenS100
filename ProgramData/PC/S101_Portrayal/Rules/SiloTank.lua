-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155
-- #256

-- Silo/tank main entry point.
function SiloTank(feature, featurePortrayal, contextParameters)
	local viewingGroup = 32220
	
	if feature.visualProminence == 1 then
		viewingGroup = 22220
	end
	
	-- #256
	if feature.inTheWater then
		viewingGroup = 12200
	end
	
	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
	end

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart symbols are the same
		if feature.categoryOfSiloTank == 1 and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:SILBUI11')
		elseif feature.categoryOfSiloTank == 2 and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif feature.categoryOfSiloTank == 3 and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TOWERS03')
		elseif feature.categoryOfSiloTank == 4 and feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TOWERS12')
		elseif feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif feature.categoryOfSiloTank == 1 then
			featurePortrayal:AddInstructions('PointInstruction:SILBUI01')
		elseif feature.categoryOfSiloTank == 2 then
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		elseif feature.categoryOfSiloTank == 3 then
			featurePortrayal:AddInstructions('PointInstruction:TOWERS01')
		elseif feature.categoryOfSiloTank == 4 then
			featurePortrayal:AddInstructions('PointInstruction:TOWERS02')
		else
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		end
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
