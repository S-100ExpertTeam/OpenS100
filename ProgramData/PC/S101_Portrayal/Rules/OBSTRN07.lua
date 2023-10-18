-- OBSTRN07 conditional symbology rules file.
-- #72
-- #89
-- #90
-- #119

-- Referenced CSPs.
require 'DEPVAL02'
require 'QUAPNT02'
require 'UDWHAZ05'
require 'SNDFRM04'

-- Main entry point for CSP.
function OBSTRN07(feature, featurePortrayal, contextParameters, originalViewingGroup)
	Debug.StartPerformance('Lua Code - OBSTRN07')

	local DEPTH_VALUE = feature.valueOfSounding or feature.defaultClearanceDepth

	if not DEPTH_VALUE then
		local LEAST_DEPTH, SEABED_DEPTH = DEPVAL02(feature)
		if LEAST_DEPTH then
			DEPTH_VALUE = LEAST_DEPTH
		elseif feature['!categoryOfObstruction'] == 6 or feature.waterLevelEffect == 3 then
			DEPTH_VALUE = CreateScaledDecimal(1, 2)
		elseif feature.waterLevelEffect == 5 then
			DEPTH_VALUE = scaledDecimalZero
		else
			DEPTH_VALUE = CreateScaledDecimal(-150, 1)
		end
	end

	local hazardSymbol, viewingGroup = UDWHAZ05(feature, featurePortrayal, contextParameters, DEPTH_VALUE, originalViewingGroup)
	
	local valueOfSounding = feature.valueOfSounding

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Continuation A
		local qualitySymbol = QUAPNT02(feature, featurePortrayal, contextParameters)

		if hazardSymbol then
			featurePortrayal:AddInstructions('PointInstruction:' .. hazardSymbol)

			if qualitySymbol then
				featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..	',31011,accuracy;PointInstruction:' .. qualitySymbol)
			end
		else
			local sounding = false

			if valueOfSounding then
				if valueOfSounding <= contextParameters.SafetyDepth then
					if feature.Code == 'UnderwaterAwashRock' then
						if contains(feature.waterLevelEffect, {4, 5}) then
							featurePortrayal:AddInstructions('PointInstruction:UWTROC04')
						else
							featurePortrayal:AddInstructions('PointInstruction:DANGER01')
							sounding = true
						end
					else
						if feature.categoryOfObstruction and feature.categoryOfObstruction == 6 then
							featurePortrayal:AddInstructions('PointInstruction:DANGER01')
							sounding = true
						elseif contains(feature.waterLevelEffect, {1, 2}) then
							featurePortrayal:AddInstructions('PointInstruction:OBSTRN11')
						elseif contains(feature.waterLevelEffect, {4, 5}) then
							featurePortrayal:AddInstructions('PointInstruction:DANGER03')
							sounding = true
						else
							featurePortrayal:AddInstructions('PointInstruction:DANGER01')
							sounding = true
						end	
					end
				else
					featurePortrayal:AddInstructions('PointInstruction:DANGER02')
					sounding = true
				end
			else
				if feature.Code == 'UnderwaterAwashRock' then
					if feature.waterLevelEffect == 3 then
						featurePortrayal:AddInstructions('PointInstruction:UWTROC03')
					else
						featurePortrayal:AddInstructions('PointInstruction:UWTROC04')
					end
				else
					if feature.categoryOfObstruction and feature.categoryOfObstruction == 6 then
						featurePortrayal:AddInstructions('PointInstruction:OBSTRN01')
					elseif contains(feature.waterLevelEffect, {1, 2}) then
						featurePortrayal:AddInstructions('PointInstruction:OBSTRN11')
					elseif contains(feature.waterLevelEffect, {4, 5}) then
						featurePortrayal:AddInstructions('PointInstruction:OBSTRN03')
					else
						featurePortrayal:AddInstructions('PointInstruction:OBSTRN01')
					end
				end
			end

			if sounding then
				local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, feature.Point, valueOfSounding)

				for _, symbol in ipairs(symbols) do
					featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
				end
			end

			if qualitySymbol then
				featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',31011,accuracy;PointInstruction:' .. qualitySymbol)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		-- Continuation B

		for curveAssociation in feature:GetFlattenedSpatialAssociations() do
			featurePortrayal:AddSpatialReference(curveAssociation)

			local spatialQuality = curveAssociation:GetInformationAssociation(unpack(sqParams))
			local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

			if qualityOfPosition and qualityOfPosition >= 2 and qualityOfPosition <= 9 then
				if hazardSymbol then
					featurePortrayal:AddInstructions('LineInstruction:LOWACC41')
				else
					featurePortrayal:AddInstructions('LineInstruction:LOWACC31')
				end
			elseif hazardSymbol or (not valueOfSounding) or (valueOfSounding and valueOfSounding <= contextParameters.SafetyDepth) then
				featurePortrayal:SimpleLineStyle('dot',0.64,'CHBLK')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				-- #245 changed color from CHBLK to CHGRD for consistency with surface symbolization. ENCWG notified
				-- about inconsistency in S-52 PL 4.0.3 OBSTRN07 CSP.
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			end

			featurePortrayal:AddInstructions('ClearGeometry')
		end

		if hazardSymbol then
			featurePortrayal:AddInstructions('LinePlacement:Relative,0.5;PointInstruction:' .. hazardSymbol)
		elseif valueOfSounding then
			local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, nil, valueOfSounding)

			for _, symbol in ipairs(symbols) do
				featurePortrayal:AddInstructions('LinePlacement:Relative,0.5;PointInstruction:' .. symbol)
			end
		end
	else
		-- Continuation C
		if hazardSymbol then
			featurePortrayal:AddInstructions('ColorFill:DEPVS;AreaFillReference:FOULAR01')
			featurePortrayal:SimpleLineStyle('dot',0.64,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:' .. hazardSymbol)
		elseif valueOfSounding then
			if valueOfSounding <= contextParameters.SafetyDepth then
				featurePortrayal:SimpleLineStyle('dot',0.64,'CHBLK')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				-- #245 changed color from CHBLK to CHGRD to match S-52
				featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			end

			local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, nil, valueOfSounding)

			for _, symbol in ipairs(symbols) do
				featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
			end
		else
			if feature.Code == 'Obstruction' and feature.categoryOfObstruction and feature.categoryOfObstruction == 6 then
				featurePortrayal:AddInstructions('AreaFillReference:FOULAR01')
				featurePortrayal:SimpleLineStyle('dot',0.64,'CHBLK')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			elseif contains(feature.waterLevelEffect, {1, 2}) then
				featurePortrayal:AddInstructions('ColorFill:CHBRN')
				-- #245 changed color from CHBLK to CSTLN to match S-52
				featurePortrayal:SimpleLineStyle('solid',0.64,'CSTLN')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			elseif feature.waterLevelEffect == 4 then
				featurePortrayal:AddInstructions('ColorFill:DEPIT')
				-- #245 changed color from CHBLK to CSTLN to match S-52
				featurePortrayal:SimpleLineStyle('dash',0.64,'CSTLN')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('ColorFill:DEPVS')
				featurePortrayal:SimpleLineStyle('dot',0.64,'CHBLK')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			end
		end
				
		local qualitySymbol = QUAPNT02(feature, featurePortrayal, contextParameters)

		if qualitySymbol then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',31011,accuracy;PointInstruction:' .. qualitySymbol)
		end
	end

	Debug.StopPerformance('Lua Code - OBSTRN07')
	return viewingGroup
end
