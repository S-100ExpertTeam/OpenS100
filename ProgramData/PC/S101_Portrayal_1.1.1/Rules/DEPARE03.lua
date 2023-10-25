-- DEPARE03 conditional symbology rules file.
-- #119

-- Referenced CSPs.
require 'RESCSP03'
require 'SAFCON01'
require 'SEABED01'

local sdMinus1 = CreateScaledDecimal(-1, 0)

-- Main entry point for CSP.
function DEPARE03(feature, featurePortrayal, contextParameters, viewingGroup)
	Debug.StartPerformance('Lua Code - DEPARE03')

	local depthRangeMinimumValue = feature.depthRangeMinimumValue or sdMinus1
	local depthRangeMaximumValue = feature.depthRangeMaximumValue
	
	SEABED01(feature, featurePortrayal, contextParameters, depthRangeMinimumValue, depthRangeMaximumValue)

	if feature.Code == 'DredgedArea' then
		featurePortrayal:AddInstructions('AreaFillReference:DRGARE01')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		if #feature.restriction > 0 then
			RESCSP03(feature, featurePortrayal, contextParameters)
		end
	end

	Debug.StopPerformance('Lua Code - DEPARE03')
	Debug.StartPerformance('Lua Code - DEPARE03 GetFlattenedSpatialAssociations')
	for curveAssociation in feature:GetFlattenedSpatialAssociations() do
		Debug.StopPerformance('Lua Code - DEPARE03 GetFlattenedSpatialAssociations')
		Debug.StartPerformance('Lua Code - DEPARE03')
		local associatedFeatures = curveAssociation.AssociatedFeatures

		local sharedFeatures = {}

		for _, sf in ipairs(associatedFeatures) do
			-- Don't include self.
			if sf ~= feature then
				sharedFeatures[sf.Code] = sf
			end
		end

		local function FirstShared(codeList)
			for _, code in ipairs(codeList) do
				local shared = sharedFeatures[code]

				if shared then
					return shared
				end
			end
		end

		local safe = false
		local unsafe = false
		local loc_safety = false

		if depthRangeMinimumValue < contextParameters.SafetyContour then
			unsafe = true
		else
			safe = true
		end

		local sharedDepthContour = sharedFeatures['DepthContour']
		local loc_valdco

		if sharedDepthContour then
			loc_valdco = sharedDepthContour.valueOfDepthContour or scaledDecimalZero
		end

		if loc_valdco == contextParameters.SafetyContour then
			loc_safety = true
		else
			local sharedDepthDredgedArea = sharedFeatures['DepthArea'] or sharedFeatures['DredgedArea']

			if sharedDepthDredgedArea then
				local drval1 = sharedDepthDredgedArea.depthRangeMinimumValue or sdMinus1

				if drval1 < contextParameters.SafetyContour then
					unsafe = true
				else
					safe = true
				end
			else
				local group1Shared = FirstShared{'LandArea', 'UnsurveyedArea', 'DockArea', 'LockBasin'}
				local landUnsurvedShared = FirstShared{'LandArea', 'UnsurveyedArea'}
				local inlandWaterShared = FirstShared{'River', 'Lake', 'Canal', 'DockArea', 'LockBasin'}
				local sharedLinearStructure = FirstShared{'LandArea', 'Gate', 'Dam', 'ShorelineConstruction', 'Causeway'}

				-- Only Dam and ShorelineConstruction in S-101 have waterLevelEffect.
				local waterLevelEffect = sharedLinearStructure and sharedLinearStructure['!waterLevelEffect']

				local watlev = sharedLinearStructure and (waterLevelEffect == nil or contains(waterLevelEffect, {1, 2, 6}))

				-- NOTE: S-52 PL 3.4 disagrees with PL 4.0.1 on the inlandWaterShared and watlev.
				--       Going with 3.4 since 4.0.1 doesn't apply the proper weight when safe water is
				--       next to a land area.
				if group1Shared and landUnsurvedShared and (not inlandWaterShared) and watlev then
					unsafe = true
				end
			end
		end

		if loc_safety or unsafe and safe then
			if unsafe then
				featurePortrayal:AddInstructions('AlertReference:SafetyContour')
			end

			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:13010;DrawingPriority:24;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:13010;DrawingPriority:24;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddSpatialReference(curveAssociation)
			
			local spatialQuality = curveAssociation:GetInformationAssociation(unpack(sqParams))
			local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

			if qualityOfPosition and qualityOfPosition ~= 1 and qualityOfPosition ~= 10 and qualityOfPosition ~= 11 then
				featurePortrayal:SimpleLineStyle('dash',0.64,'DEPSC')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:SimpleLineStyle('solid',0.64,'DEPSC')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			end

			if loc_valdco then
				if contextParameters.RadarOverlay then
					featurePortrayal:AddInstructions('ViewingGroup:33021,contourLabel;DrawingPriority:24;DisplayPlane:OverRADAR')
				else
					featurePortrayal:AddInstructions('ViewingGroup:33021,contourLabel;DrawingPriority:24;DisplayPlane:UnderRADAR')
				end

				featurePortrayal:AddInstructions('LinePlacement:Relative,0.5')

				local instructions = SAFCON01(contextParameters, loc_valdco)

				for _, instruction in ipairs(instructions) do
					featurePortrayal:AddInstructions(instruction)
				end
			end

			featurePortrayal:AddInstructions('ClearGeometry')

			featurePortrayal:AddInstructions('AlertReference')
		end
	end

	Debug.StopPerformance('Lua Code - DEPARE03')
end
