-- DEPCNT03 conditional symbology rules file.
-- #119

-- Referenced CSPs.
require 'SAFCON01'

local selectedSafetyContour
local computedSafetyContour

local function ComputeSafetyContour(safetyContour)
	if safetyContour == selectedSafetyContour then
		return computedSafetyContour
	end

	computedSafetyContour = nil

	local depthContours = portrayalContext:GetFeatures('DepthContour')

	for i, dc in ipairs(depthContours) do
		if dc.valueOfDepthContour and dc.valueOfDepthContour >= safetyContour then
			computedSafetyContour = computedSafetyContour or dc.valueOfDepthContour

			if dc.valueOfDepthContour < computedSafetyContour then
				computedSafetyContour = dc.valueOfDepthContour
			end
		end
	end

	selectedSafetyContour = safetyContour
	return computedSafetyContour
end

-- Main entry point for CSP.
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
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',33022,contourLabel')

		featurePortrayal:AddInstructions('LinePlacement:Relative,0.5')
		local instructions = SAFCON01(contextParameters, feature.valueOfDepthContour or scaledDecimalZero)
		for i, instruction in ipairs(instructions) do
			featurePortrayal:AddInstructions(instruction)
		end
	end

	local safetyContour = ComputeSafetyContour(contextParameters.SafetyContour)

	if feature.valueOfDepthContour ~= safetyContour then
		GenerateCurves()
		GenerateLabels()
	else
		featurePortrayal:AddInstructions('NullInstruction')
	end

	Debug.StopPerformance('Lua Code - DEPCNT03')
end
