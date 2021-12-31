-- SAFCON01 conditional symbology rules file.

local sd99999 = CreateScaledDecimal(99999, 0)

-- Main entry point for CSP.
function SAFCON01(contextParameters, depthValue)
	Debug.StartPerformance('Lua Code - SAFCON01')

	local instructions = {}

	if depthValue < scaledDecimalZero or depthValue > sd99999 then
		Debug.StopPerformance('Lua Code - SAFCON01')
		return {}
	end

	local sign, depth, fractional = depthValue:Split()

	if fractional == '' then
		fractional = '0'
	end

	local idepth = tonumber(depth)
	local ifractional = tonumber(fractional)

	local function add(symbol) instructions[#instructions + 1] = 'PointInstruction:SAFCON' .. symbol end

	if idepth < 10 and ifractional ~= 0 then
		add(0 .. depth)
		add(6 .. string.sub(fractional, 1, 1))
	elseif idepth < 10 then
		add(0 .. depth)
	elseif idepth < 31 and ifractional ~= 0 then
		add(2 .. string.sub(depth, 1, 1))
		add(1 .. string.sub(depth, 2, 2))
		add(5 .. string.sub(fractional, 1, 1))
	elseif idepth < 100 then
		add(2 .. string.sub(depth, 1, 1))
		add(1 .. string.sub(depth, 2, 2))
	--[[ Manufacturer optional.  No SVGs available for this section
	elseif ...
	--]]
	end

	Debug.StopPerformance('Lua Code - SAFCON01')
	return instructions
end
