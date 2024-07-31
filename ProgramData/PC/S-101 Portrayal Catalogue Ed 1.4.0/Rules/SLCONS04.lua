-- SLCONS04 conditional symbology rules file.
-- #78
-- #90
-- #119

-- Referenced CSPs.
require 'QUAPNT02'

-- Main entry point for CSP.
function SLCONS04(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - SLCONS04')

	if feature.PrimitiveType == PrimitiveType.Point then
		Debug.StopPerformance('Lua Code - SLCONS04')
		local symbol = QUAPNT02(feature, featurePortrayal, contextParameters)
		Debug.StartPerformance('Lua Code - SLCONS04')

		if symbol then
			--Debug.Break()
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',90011;PointInstruction:' .. symbol)
		end
	else
		Debug.StopPerformance('Lua Code - SLCONS04')
		Debug.StartPerformance('Lua Code - SLCONS04 GetFlattenedSpatialAssociations')
		for curveAssociation in feature:GetFlattenedSpatialAssociations() do
			Debug.StopPerformance('Lua Code - SLCONS04 GetFlattenedSpatialAssociations')
			Debug.StartPerformance('Lua Code - SLCONS04')
			featurePortrayal:AddSpatialReference(curveAssociation)

			local spatialQuality = curveAssociation:GetInformationAssociation(unpack(sqParams))
			local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

			if qualityOfPosition and qualityOfPosition ~=1 and qualityOfPosition ~= 10 and qualityOfPosition ~= 11 then
				featurePortrayal:AddInstructions('LineInstruction:LOWACC21')
			else
				local condition = feature.condition
				local categoryOfShorelineConstruction = feature.categoryOfShorelineConstruction
				local waterLevelEffect = feature.waterLevelEffect

				if contains(condition, {1, 2}) then
					featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				elseif contains(categoryOfShorelineConstruction, {6, 15, 16, 22}) then
					featurePortrayal:SimpleLineStyle('solid',1.28,'CSTLN')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				elseif contains(waterLevelEffect, {3, 4}) then
					featurePortrayal:SimpleLineStyle('dash',0.64,'CSTLN')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				else
					featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				end
			end

			featurePortrayal:AddInstructions('ClearGeometry')
		end
	end

	Debug.StopPerformance('Lua Code - SLCONS04')
end
