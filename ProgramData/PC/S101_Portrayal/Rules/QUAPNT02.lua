-- QUAPNT02 conditional symbology rules file.

-- Main entry point for CSP.
function QUAPNT02(feature, featurePortrayal, contextParameters)
	Debug.StartPerformance('Lua Code - QUAPNT02')

	for spatialAssociation in feature:GetFlattenedSpatialAssociations() do
		if IsLowAccuracy(spatialAssociation) then
			Debug.StopPerformance('Lua Code - QUAPNT02')
			return 'LOWACC01'
		end
	end

	Debug.StopPerformance('Lua Code - QUAPNT02')
end

-- Returns true if horizontal component of the spatial is approximate
function IsLowAccuracy(spatialAssociation)
	Debug.StartPerformance('Lua Code - IsLowAccuracy')
	
	local spatialQuality = spatialAssociation:GetInformationAssociation(unpack(sqParams))
	local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

	-- Note: Check is from S-52
	if qualityOfPosition and qualityOfPosition >= 2 and qualityOfPosition <= 9 then
		Debug.StopPerformance('Lua Code - IsLowAccuracy')
		return true
	end
	
	Debug.StopPerformance('Lua Code - IsLowAccuracy')
	return false
end

function AddLowAccuracySymbol(feature, featurePortrayal, viewingGroup)
	Debug.StartPerformance('Lua Code - QUAPNT02')

	local spatialAssociations = feature:GetSpatialAssociations()
	
	if #spatialAssociations == 1 then
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..	',90011;PointInstruction:LOWACC01')
	else
		local lowAccuracyAssociations = {}
		
		for _, spatialAssociation in ipairs(spatialAssociations) do
			for _, spatialComponent in ipairs(feature:FlattenSpatialAssociation(spatialAssociation)) do
				if IsLowAccuracy(spatialComponent) then
					lowAccuracyAssociations[#lowAccuracyAssociations + 1] = spatialAssociation
					break
				end
			end
		end
		
		if #spatialAssociations ~= #lowAccuracyAssociations then
			for _, spatialAssociation in ipairs(lowAccuracyAssociations) do
				featurePortrayal:AddSpatialReference(spatialAssociation)
			end
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..	',90011;PointInstruction:LOWACC01;ClearGeometry')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..	',90011;PointInstruction:LOWACC01')
		end
	end

	Debug.StopPerformance('Lua Code - QUAPNT02')
end
