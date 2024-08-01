-- SNDFRM04 conditional symbology rules file.

-- PSWG #118, PC #141
-- PC #307

-- Main entry point for CSP.
function SNDFRM04(feature, featurePortrayal, contextParameters, soundingPoint, depthValue)
	Debug.StartPerformance('Lua Code - SNDFRM04')

	CheckType(depthValue, 'ScaledDecimal')

	local symbols = {}

	local function addSymbol(symbol)
		symbols[#symbols + 1] = symbol
	end

	local symbolPrefix
	local colour
	local fontSize
	local lowAccuracySymbolRing

	if depthValue <= contextParameters.SafetyDepth then
		colour = 'SNDG2' -- Sounding shallow
		symbolPrefix = 'SOUNDS'
		lowAccuracySymbolRing = 'C3'
		--fontSize = contextParameters._Testing_SoundingsAsText_SizeUnsafe
	else
		colour = 'SNDG1' -- Sounding deep
		symbolPrefix = 'SOUNDG'
		lowAccuracySymbolRing = 'C2'
		--fontSize = contextParameters._Testing_SoundingsAsText_SizeSafe
	end

	-- TODO: if attribute is not present, check value from intersecting meta-feature QualityOfSurvey
	if contains(feature.techniqueOfVerticalMeasurement, { 4, 18 }) then
		-- add swept sounding symbol
		addSymbol(symbolPrefix..'B1')
	end

	if feature.Code == 'DepthNoBottomFound' or contains(feature.qualityOfVerticalMeasurement, { 3, 4, 5, 8, 9 }) or contains(feature.status, { 18 }) then
		addSymbol(symbolPrefix.. lowAccuracySymbolRing)
	else
		local spatialQuality = feature:GetSpatialAssociation():GetInformationAssociation(unpack(sqParams))
		local qualityOfPosition = spatialQuality and spatialQuality.qualityOfHorizontalMeasurement

		if qualityOfPosition and qualityOfPosition ~= 1 and qualityOfPosition ~= 10 and qualityOfPosition ~= 11 then
			addSymbol(symbolPrefix.. lowAccuracySymbolRing)
		end
	end
	
	local sign, depth, fractional = depthValue:Split()

	if fractional == '' then
		fractional = '0'
	end

	local idepth = tonumber(depth)
	local ifractional = tonumber(fractional)

	if sign == '-' then
		addSymbol(symbolPrefix..'A1')
	end

	if idepth < 10 then
		addSymbol(symbolPrefix..'1'..depth)
		addSymbol(symbolPrefix..'5'..string.sub(fractional, 1, 1))
	elseif idepth < 31 and ifractional ~= 0 then
		addSymbol(symbolPrefix..'2'..string.sub(depth, 1, 1))
		addSymbol(symbolPrefix..'1'..string.sub(depth, 2, 2))
		addSymbol(symbolPrefix..'5'..string.sub(fractional, 1, 1))
	elseif idepth < 100 then
		addSymbol(symbolPrefix..'1'..string.sub(depth, 1, 1))
		addSymbol(symbolPrefix..'0'..string.sub(depth, 2, 2))
	elseif idepth < 1000 then
		addSymbol(symbolPrefix..'2'..string.sub(depth, 1, 1))
		addSymbol(symbolPrefix..'1'..string.sub(depth, 2, 2))
		addSymbol(symbolPrefix..'0'..string.sub(depth, 3, 3))
	elseif idepth < 10000 then
		addSymbol(symbolPrefix..'2'..string.sub(depth, 1, 1))
		addSymbol(symbolPrefix..'1'..string.sub(depth, 2, 2))
		addSymbol(symbolPrefix..'0'..string.sub(depth, 3, 3))
		addSymbol(symbolPrefix..'4'..string.sub(depth, 4, 4))
	else
		addSymbol(symbolPrefix..'3'..string.sub(depth, 1, 1))
		addSymbol(symbolPrefix..'2'..string.sub(depth, 2, 2))
		addSymbol(symbolPrefix..'1'..string.sub(depth, 3, 3))
		addSymbol(symbolPrefix..'0'..string.sub(depth, 4, 4))
		addSymbol(symbolPrefix..'4'..string.sub(depth, 5, 5))
	end

	Debug.StopPerformance('Lua Code - SNDFRM04')

	return symbols
end
