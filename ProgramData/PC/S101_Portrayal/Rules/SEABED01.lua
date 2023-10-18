-- SEABED01 conditional symbology rules file.
-- #155: Update context parameters
-- #119: Independent Mariner Selections

-- Main entry point for CSP.
function SEABED01(feature, featurePortrayal, contextParameters, depthRangeMinimumValue, depthRangeMaximumValue)
	Debug.StartPerformance('Lua Code - SEABED01')

	local Colour = 'DEPIT'
	local Shallow = true

	if (not contextParameters.FourShades) then
		-- Two shades: DEPVS / DEPDW
		if (depthRangeMinimumValue >= scaledDecimalZero and (not depthRangeMaximumValue or depthRangeMaximumValue > scaledDecimalZero)) then
			Colour = 'DEPVS'
		end

		if (depthRangeMinimumValue >= contextParameters.SafetyContour and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.SafetyContour)) then
			Colour = 'DEPDW'
			Shallow = false
		end
	else
		-- Four shades: DEPVS / DEPMS / DEPMD / DEPDW
		if (depthRangeMinimumValue >= scaledDecimalZero and (not depthRangeMaximumValue or depthRangeMaximumValue > scaledDecimalZero)) then
			Colour = 'DEPVS'
		end

		if (depthRangeMinimumValue >= contextParameters.ShallowContour and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.ShallowContour)) then
			Colour = 'DEPMS'
		end

		if (depthRangeMinimumValue >= contextParameters.SafetyContour and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.SafetyContour)) then
			Colour = 'DEPMD'
			Shallow = false
		end

		if (depthRangeMinimumValue >= contextParameters.DeepContour and (not depthRangeMaximumValue or depthRangeMaximumValue > contextParameters.DeepContour)) then
			Colour = 'DEPDW'
			Shallow = false
		end
	end

	--Debug.Trace('SEABED01: Colour = ' .. Colour .. ' SafetyContour = ' .. contextParameters.SafetyContour .. ' depthRangeMinimumValue = ' .. depthRangeMinimumValue .. ' depthRangeMaximumValue = ' .. depthRangeMaximumValue)

	if Shallow then
		featurePortrayal:AddInstructions('AlertReference:SafetyContour')
	end

	featurePortrayal:AddInstructions('ColorFill:' .. Colour)

	if Shallow then
		-- S-52 10.5.7
		featurePortrayal:AddInstructions('ViewingGroup:23010,shallowPattern;DrawingPriority:9;DisplayPlane:UnderRADAR')

		featurePortrayal:AddInstructions('AreaFillReference:DIAMOND1')

		-- Restore default viewing and priorities for depth area and dredged area.
		featurePortrayal:AddInstructions('ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRADAR')
	end

	if Shallow then
		featurePortrayal:AddInstructions('AlertReference')
	end

	Debug.StopPerformance('Lua Code - SEABED01')
end
