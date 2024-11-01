-- QUAPOS01 conditional symbology rules file.
-- #90
-- #119

-- Referenced CSPs.
require 'QUAPNT02'
require 'QUALIN02'

-- Main entry point for CSP.
function QUAPOS01(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - QUAPOS01')

	if feature.PrimitiveType == PrimitiveType.Curve then
		QUALIN02(feature, featurePortrayal, contextParameters)
	else
		local symbol = QUAPNT02(feature, featurePortrayal, contextParameters)

		if symbol then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..	',90011;PointInstruction:' .. symbol)
		end
	end

	Debug.StopPerformance('Lua Code - QUAPOS01')
end
