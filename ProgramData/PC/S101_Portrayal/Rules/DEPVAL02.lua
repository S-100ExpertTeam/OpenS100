-- DEPVAL02 conditional symbology rules file.
-- #72

-- Main entry point for CSP.
function DEPVAL02(feature)
	Debug.StartPerformance('Lua Code - DEPVAL02')
	
	-- TODO:
	-- Currently surrounding depth encodes the depth range minimum value of the *deeper* of the depth areas
	-- covering the feature. Update to use the *shoalest* of the depth areas once the DCEG/FC have been updated.
	local LEAST_DEPTH = feature.surroundingDepth
	local SEABED_DEPTH = nil
	
	if LEAST_DEPTH == nil
	then
		Debug.StopPerformance('Lua Code - DEPVAL02')
		return LEAST_DEPTH, SEABED_DEPTH
	end
	
	SEABED_DEPTH = LEAST_DEPTH
	if feature.waterLevelEffect == 3 and (feature.expositionOfSounding == 1 or feature.expositionOfSounding == 3)
	then
		Debug.StopPerformance('Lua Code - DEPVAL02')
		return LEAST_DEPTH, SEABED_DEPTH
	end

	LEAST_DEPTH = nil
	Debug.StopPerformance('Lua Code - DEPVAL02')
	return LEAST_DEPTH, SEABED_DEPTH
end
