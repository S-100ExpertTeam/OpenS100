-- SEABED01 conditional symbology rules file.

-- Main entry point for CSP.
function SEABED01(feature, featurePortrayal, contextParameters, depthRangeMinimumValue, depthRangeMaximumValue)
	Debug.StartPerformance('Lua Code - SEABED01')

	local Colour = 'DEPIT'
	local Shallow = true

	if (contextParameters.TWO_SHADES) then
		if (depthRangeMinimumValue >= scaledDecimalZero and (not depthRangeMaximumValue or depthRangeMaximumValue > scaledDecimalZero)) then
			Colour = 'DEPVS'
		end

		if (depthRangeMinimumValue >= contextParameters.SAFETY_CONTOUR and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.SAFETY_CONTOUR)) then
			Colour = 'DEPDW'
			Shallow = false
		end
	else
		if (depthRangeMinimumValue >= scaledDecimalZero and (not depthRangeMaximumValue or depthRangeMaximumValue > scaledDecimalZero)) then
			Colour = 'DEPVS'
		end

		if (depthRangeMinimumValue >= contextParameters.SHALLOW_CONTOUR and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.SHALLOW_CONTOUR)) then
			Colour = 'DEPMS'
		end

		if (depthRangeMinimumValue >= contextParameters.SAFETY_CONTOUR and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.SAFETY_CONTOUR)) then
			Colour = 'DEPMD'
			Shallow = false
		end

		if (depthRangeMinimumValue >= contextParameters.DEEP_CONTOUR and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.DEEP_CONTOUR)) then
			Colour = 'DEPDW'
			Shallow = false
		end
	end

	--Debug.Trace('SEABED01: Colour = ' .. Colour .. ' SAFETY_CONTOUR = ' .. contextParameters.SAFETY_CONTOUR .. ' depthRangeMinimumValue = ' .. depthRangeMinimumValue .. ' depthRangeMaximumValue = ' .. depthRangeMaximumValue)

	featurePortrayal:AddInstructions('ColorFill:' .. Colour)

	if (contextParameters.SHALLOW_PATTERN and Shallow) then
		featurePortrayal:AddInstructions('AreaFillReference:DIAMOND1')
	end

	Debug.StopPerformance('Lua Code - SEABED01')
end
