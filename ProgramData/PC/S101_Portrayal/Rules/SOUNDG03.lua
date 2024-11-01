-- SOUNDG03 conditional symbology rules file.

-- Referenced CSPs.
require 'SNDFRM04'

-- Main entry point for CSP.
function SOUNDG03(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - SOUNDG03')

	local points = feature.MultiPoint.Points

	for i, point in ipairs(points) do
		Debug.StopPerformance('Lua Code - SOUNDG03')
		local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, point, point.ScaledZ)
		Debug.StartPerformance('Lua Code - SOUNDG03')

		if point.ScaledZ <= contextParameters.SafetyContour then
			featurePortrayal:AddInstructions('AlertReference:NavHazard')
		end

		featurePortrayal:AddInstructions('AugmentedPoint:GeographicCRS,' .. point.X .. ',' .. point.Y)

		for j, symbol in ipairs(symbols) do
			featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
		end

		-- Clear for next sounding.
		featurePortrayal:AddInstructions('AlertReference')
	end
	
	featurePortrayal:AddInstructions('ClearGeometry')

	Debug.StopPerformance('Lua Code - SOUNDG03')
end
