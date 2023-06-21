-- QUAPOS01 conditional symbology rules file.

-- Referenced CSPs.
require 'QUAPNT02'
require 'QUALIN02'

-- Main entry point for CSP.
function QUAPOS01(feature, featurePortrayal, contextParameters)
	Debug.StartPerformance('Lua Code - QUAPOS01')

	if feature.PrimitiveType == PrimitiveType.Curve then
		QUALIN02(feature, featurePortrayal, contextParameters)
	else
		local symbol = QUAPNT02(feature, featurePortrayal, contextParameters)

		if symbol then
			featurePortrayal:AddInstructions('ViewingGroup:12210;PointInstruction:' .. symbol)
		end
	end

	Debug.StopPerformance('Lua Code - QUAPOS01')
end
