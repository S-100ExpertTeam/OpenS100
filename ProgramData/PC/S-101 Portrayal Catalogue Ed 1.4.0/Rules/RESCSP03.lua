-- RESCSP03 conditional symbology rules file.

-- Main entry point for CSP.
function RESCSP03(feature, featurePortrayal, contextParameters)
	Debug.StartPerformance('Lua Code - RESCSP03')

	local restriction = feature.restriction

	if contains(restriction, { 7, 8, 14 }) then
		-- Entry restricted or prohibited
		if contains(restriction, { 1, 2, 3, 4, 5, 6, 13, 16, 17, 23, 24, 25, 26, 27 }) then
			featurePortrayal:AddInstructions('PointInstruction:ENTRES61')
		elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
			featurePortrayal:AddInstructions('PointInstruction:ENTRES71')
		else
			featurePortrayal:AddInstructions('PointInstruction:ENTRES51')
		end
	elseif contains(restriction, { 1, 2 }) then
		-- Anchoring restricted or prohibited
		if contains(restriction, { 3, 4, 5, 6, 13, 16, 17, 23, 24, 25, 26, 27 }) then
			featurePortrayal:AddInstructions('PointInstruction:ACHRES61')
		elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
			featurePortrayal:AddInstructions('PointInstruction:ACHRES71')
		else
			featurePortrayal:AddInstructions('PointInstruction:ACHRES51')
		end
	elseif contains(restriction, { 3, 4, 5, 6, 24 }) then
		-- Fishing restricted or prohibited
		if contains(restriction, { 13, 16, 17, 23, 24, 25, 26, 27 }) then
			featurePortrayal:AddInstructions('PointInstruction:FSHRES61')
		elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
			featurePortrayal:AddInstructions('PointInstruction:FSHRES71')
		else
			featurePortrayal:AddInstructions('PointInstruction:FSHRES51')
		end
	elseif contains(restriction, { 13, 16, 17, 23, 25, 26, 27 }) then
		-- Own ship restrictions
		if contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
			featurePortrayal:AddInstructions('PointInstruction:CTYARE71')
		else
			featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
		end
	elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
		featurePortrayal:AddInstructions('PointInstruction:INFARE51')
	else
		featurePortrayal:AddInstructions('PointInstruction:RSRDEF51')
	end

	Debug.StopPerformance('Lua Code - RESCSP03')
end
