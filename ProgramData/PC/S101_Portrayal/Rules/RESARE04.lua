-- RESARE04 conditional symbology rules file.
--
-- #304
-- Main entry point for CSP.
function RESARE04(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - RESARE04')
	
	local restriction = feature.restriction
	local categoryOfRestrictedArea = feature.categoryOfRestrictedArea

	if restriction then
		if contains(restriction, { 7, 8, 14 }) then
			-- Continuation A.  Entry restricted or prohibited

			featurePortrayal:AddInstructions('DrawingPriority:18')

			if contains(restriction, { 1, 2, 3, 4, 5, 6, 13, 16, 17, 23, 24, 25, 26, 27, 42 }) then
				featurePortrayal:AddInstructions('PointInstruction:ENTRES61')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, { 1, 8, 9, 12, 14, 18, 19, 21, 24, 25, 26, 29 }) then
				featurePortrayal:AddInstructions('PointInstruction:ENTRES61')
			elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
				featurePortrayal:AddInstructions('PointInstruction:ENTRES71')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, {4, 5, 6, 7, 10, 20, 22, 23, 31 }) then -- Issue #53
				featurePortrayal:AddInstructions('PointInstruction:ENTRES71')
			else
				featurePortrayal:AddInstructions('PointInstruction:ENTRES51')
			end

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:ENTRES51')
			end

		elseif contains(restriction, { 1, 2 }) then
			-- Continuation B.  Anchoring restricted or prohibited

			featurePortrayal:AddInstructions('DrawingPriority:18')

			if contains(restriction, { 3, 4, 5, 6, 13, 16, 17, 23, 24, 25, 26, 27 }) then
				featurePortrayal:AddInstructions('PointInstruction:ACHRES61')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, { 1, 8, 9, 12, 14, 18, 19, 21, 24, 25, 26, 29 }) then
				featurePortrayal:AddInstructions('PointInstruction:ACHRES61')
			elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
				featurePortrayal:AddInstructions('PointInstruction:ACHRES71')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, {4, 5, 6, 7, 10, 20, 22, 23 }) then
				featurePortrayal:AddInstructions('PointInstruction:ACHRES71')
			else
				featurePortrayal:AddInstructions('PointInstruction:ACHRES51')
			end

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:ACHRES51')
			end

		elseif contains(restriction, { 3, 4, 5, 6, 24 }) then
			-- Continuation C.  Fishing restricted or prohibited

			featurePortrayal:AddInstructions('DrawingPriority:18')

			if contains(restriction, { 13, 16, 17, 23, 24, 25, 26, 27 }) then
				featurePortrayal:AddInstructions('PointInstruction:FSHRES61')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, { 1, 8, 9, 12, 14, 18, 19, 21, 24, 25, 26, 29 }) then
				featurePortrayal:AddInstructions('PointInstruction:FSHRES61')
			elseif contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
				featurePortrayal:AddInstructions('PointInstruction:FSHRES71')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, {4, 5, 6, 7, 10, 20, 22, 23 }) then
				featurePortrayal:AddInstructions('PointInstruction:FSHRES71')
			else
				featurePortrayal:AddInstructions('PointInstruction:FSHRES51')
			end

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:FSHRES51')
			end

		elseif contains(restriction, { 13, 16, 17, 23, 25, 26, 27 }) then
			-- Continuation D.  Own ship restrictions

			featurePortrayal:AddInstructions('DrawingPriority:18')

			if contains(restriction, { 9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
				featurePortrayal:AddInstructions('PointInstruction:CTYARE71')
			elseif categoryOfRestrictedArea and contains(categoryOfRestrictedArea, { 4, 5, 6, 7, 10, 20, 22, 23 }) then
				featurePortrayal:AddInstructions('PointInstruction:CTYARE71')
			else
				featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
			end

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
			end

		else
			if contains(restriction, {9, 10, 11, 12, 15, 18, 19, 20, 21, 22 }) then
				featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			else
				featurePortrayal:AddInstructions('PointInstruction:RSRDEF51')
			end

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
			end
		end
	else
		-- Continuation E.  No restriction applies

		if categoryOfRestrictedArea then
			if contains(categoryOfRestrictedArea, { 1, 8, 9, 12, 14, 18, 19, 21, 24, 25, 26, 29 }) then
				if contains(categoryOfRestrictedArea, { 4, 5, 6, 7, 10, 20, 22, 23 }) then
					featurePortrayal:AddInstructions('PointInstruction:CTYARE71')
				else
					featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
				end
			else
				if contains(categoryOfRestrictedArea, { 4, 5, 6, 7, 10, 20, 22, 23 }) then
					featurePortrayal:AddInstructions('PointInstruction:INFARE51')
				else
					featurePortrayal:AddInstructions('PointInstruction:RSRDEF51')
				end
			end
		else
			featurePortrayal:AddInstructions('PointInstruction:RSRDEF51')
		end

		if contextParameters.PlainBoundaries then
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
		end
	end

	Debug.StopPerformance('Lua Code - RESARE04')
end
