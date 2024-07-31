-- QUAPNT02 conditional symbology rules file.

-- Main entry point for CSP.
function QUAPNT02(feature, featurePortrayal, contextParameters)
	Debug.StartPerformance('Lua Code - QUAPNT02')

	for spatialAssociation in feature:GetFlattenedSpatialAssociations() do
		local spatialQuality = spatialAssociation:GetInformationAssociation(unpack(sqParams))
		local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

		if qualityOfPosition and qualityOfPosition >= 2 and qualityOfPosition <= 9 then
			Debug.StopPerformance('Lua Code - QUAPNT02')
			return 'LOWACC01'
		end
	end

	Debug.StopPerformance('Lua Code - QUAPNT02')
end
