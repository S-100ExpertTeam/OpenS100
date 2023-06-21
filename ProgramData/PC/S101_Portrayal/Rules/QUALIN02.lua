-- QUALIN02 conditional symbology rules file.

-- Main entry point for CSP.
function QUALIN02(feature, featurePortrayal, contextParameters)
	Debug.StartPerformance('Lua Code - QUALIN02')

	for curveAssociation in feature:GetFlattenedSpatialAssociations() do
		featurePortrayal:AddSpatialReference(curveAssociation)

		local spatialQuality = curveAssociation:GetInformationAssociation(unpack(sqParams))
		local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

		if qualityOfPosition and qualityOfPosition ~=1 and qualityOfPosition ~= 10 and qualityOfPosition ~= 11 then
			featurePortrayal:AddInstructions('LineInstruction:LOWACC21')
		elseif feature.Code == 'Coastline' then
			if feature.radarConspicuous == 1 then
				featurePortrayal:SimpleLineStyle('solid',0.96,'CHMGF')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			end

			featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			-- Must be LandArea
			featurePortrayal:SimpleLineStyle('solid',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end

		featurePortrayal:AddInstructions('ClearGeometry')
	end

	Debug.StopPerformance('Lua Code - QUALIN02')
end
