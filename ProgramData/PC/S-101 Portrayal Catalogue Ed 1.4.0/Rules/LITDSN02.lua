-- LITDSN02 conditional symbology rules file.
-- #189

local categoryOfLights =
{
	[1] = 'Dir ',
	[5] = 'Aero ',
	[7] = 'Fog Det Lt '
}

local lightCharacteristics = 
{
	[1]  = {[1] = 'F'},               -- Fixed
	[2]  = {[1] = 'Fl'},              -- Flashing
	[3]  = {[1] = 'LFl'},             -- long-flashing
	[4]  = {[1] = 'Q'},               -- quick-flashing
	[5]  = {[1] = 'VQ'},              -- very quick-flashing
	[6]  = {[1] = 'UQ'},              -- ultra quick-flashing
	[7]  = {[1] = 'Iso'},             -- Isophased
	[8]  = {[1] = 'Oc'},              -- Occulting
	[9]  = {[1] = 'IQ'},              -- interrupted quick-flashing
	[10] = {[1] = 'IVQ'},             -- interrupted very quick-flashing
	[11] = {[1] = 'IUQ'},             -- interrupted ultra quick-flashing
	[12] = {[1] = 'Mo'},              -- Morse
	[13] = {[1] = 'FFl'},             -- fixed/flashing
	[14] = {[1] = 'Fl', [2] = 'LFl'}, -- flash/long-flash
	[15] = {[1] = 'OcFl'},            -- occulting/flashing
	[16] = {[1] = 'FLFl'},            -- fixed/long-flash
	[17] = {[1] = 'AlOc'},            -- occulting alternating
	[18] = {[1] = 'AlLFl'},           -- long-flash alternating 
	[19] = {[1] = 'AlFl'},            -- flash alternating 
	[20] = {[1] = 'Al'},              -- group alternating
	[25] = {[1] = 'Q',  [2] = 'LFl'}, -- quick-flash plus long-flash
	[26] = {[1] = 'VQ', [2] = 'LFl'}, -- very quick-flash plus long-flash
	[27] = {[1] = 'UQ', [2] = 'LFl'}, -- ultra quick-flash plus long-flash
	[28] = {[1] = 'Al'},              -- Alternating
	[29] = {[1] = 'AlF Fl'}           -- fixed and alternating flashing
}

local colours =
{
	[1]  = 'W',
	[2]  = 'B',
	[3]  = 'R',
	[4]  = 'G',
	[5]  = 'Bu',
	[6]  = 'Y',
	[7]  = 'Gy',
	[8]  = 'Br',
	[9]  = 'Y',
	[10] = 'Vi',
	[11] = 'Y',
	[12] = 'Mag',
	[13] = 'R'
}

local statuses =
{
	[2] = '(occas)',   -- occasional
	[7] = '(temp)',    -- Temporary
	[8] = '(priv)',    -- Private
	[11] = '(exting)', -- extinguished
	[17] = '(U)'      -- un-watched
}

-- Main entry point for CSP.
function LITDSN02(categoryOfLight, rhythmOfLight, lightColour, height, valueOfNominalRange, status, valueOfNominalRange2, numRanges)
	Debug.StartPerformance('Lua Code - LITDSN02')

	local description = categoryOfLights[categoryOfLight] or ''
	local statusString = statuses[status] or ''

	if rhythmOfLight then
		-- Light Characteristic / Signal Group
		local lightCharacteristic = lightCharacteristics[rhythmOfLight.lightCharacteristic] or {}
		local signalGroup = rhythmOfLight.signalGroup

		for i = 1, #lightCharacteristic do
			if i > 1 then
				description = description .. '+'
			end

			description = description .. lightCharacteristic[i]

			if signalGroup[i] ~= nil and signalGroup[i] ~= '()' and signalGroup[i] ~= '(1)' then
				description = description .. signalGroup[i]
			end
		end
	end

	-- Colour
	local colourFound = false
	local prevColour
	for i, colour in ipairs(lightColour) do
		if colours[colour] then
			if not colourFound then
				description = description .. ' '
				colourFound = true
			end

			if prevColour ~= colours[colour] then
				description = description .. colours[colour]
				prevColour = colours[colour]
			end
		end
	end

	local space = ' '

	if rhythmOfLight then
		-- Signal Period
		local signalPeriod = rhythmOfLight.signalPeriod

		if signalPeriod then
			description = string.format('%s%s%gs', description, space, signalPeriod:ToNumber())
			space = ''
		end
	end
	
	-- Height
	if height then
		description = string.format('%s%s%gm', description, space, height:ToNumber())
		space = ''
	end

	-- Value of Nominal Range
	if valueOfNominalRange then
		if valueOfNominalRange2 and valueOfNominalRange ~= valueOfNominalRange2	then
			if numRanges and numRanges == 2 then
				description = string.format('%s%s%g/%gM', description, space, valueOfNominalRange:ToNumber(), valueOfNominalRange2:ToNumber())
			else
				description = string.format('%s%s%g-%gM', description, space, valueOfNominalRange:ToNumber(), valueOfNominalRange2:ToNumber())
			end
		else
			description = string.format('%s%s%gM', description, space, valueOfNominalRange:ToNumber())
		end
	end

	-- Status
	for i, statusValue in ipairs(status)
	do
		if statuses[statusValue]
		then
			description = description .. statuses[statusValue]
		end
	end

	Debug.StopPerformance('Lua Code - LITDSN02')

	return EncodeString(description)
end
