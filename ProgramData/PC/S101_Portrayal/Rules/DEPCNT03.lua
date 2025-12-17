require 'SAFCON01'

function DEPCNT03(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - DEPCNT03')

	local function GenerateCurves()
		Debug.StartPerformance('Lua Code - DEPCNT03 GenerateCurves')

		local dashedAssociations = {}
		local solidAssociations = {}

		for curveAssociation in feature:GetFlattenedSpatialAssociations() do
			--Debug.Break()
			Debug.StartPerformance('Lua Code - DEPCNT03 GetInformationAssociation')
			local spatialQuality = curveAssociation:GetInformationAssociation(unpack(sqParams))
			Debug.StopPerformance('Lua Code - DEPCNT03 GetInformationAssociation')
			local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

			if qualityOfPosition and qualityOfPosition ~=1 and qualityOfPosition ~= 10 and qualityOfPosition ~= 11 then
				dashedAssociations[#dashedAssociations + 1] = curveAssociation
			else
				solidAssociations[#solidAssociations + 1] = curveAssociation
			end
		end

		if #dashedAssociations > 0 then
			featurePortrayal:SimpleLineStyle('dash',0.32,'DEPCN')

			for _, assoc in ipairs(dashedAssociations) do
				featurePortrayal:AddSpatialReference(assoc)
			end

			featurePortrayal:AddInstructions('LineInstruction:_simple_;ClearGeometry')
		end

		if #solidAssociations > 0 then
			featurePortrayal:SimpleLineStyle('solid',0.32,'DEPCN')

			for _, assoc in ipairs(solidAssociations) do
				featurePortrayal:AddSpatialReference(assoc)
			end

			featurePortrayal:AddInstructions('LineInstruction:_simple_;ClearGeometry')
		end

		Debug.StopPerformance('Lua Code - DEPCNT03 GenerateCurves')
	end

	local function GenerateLabels()
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',90031')

		featurePortrayal:AddInstructions('LinePlacement:Relative,0.5,,true')
		local instructions = SAFCON01(contextParameters, feature.valueOfDepthContour or scaledDecimalZero)
		for i, instruction in ipairs(instructions) do
			featurePortrayal:AddInstructions(instruction)
		end
	end

	GenerateCurves()
	GenerateLabels()

	Debug.StopPerformance('Lua Code - DEPCNT03')
end
