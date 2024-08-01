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
	elseif idepth < 1000 then
		add(8 .. string.sub(depth, 1, 1))
		add(0 .. string.sub(depth, 2, 2))
		add(9 .. string.sub(depth, 3, 3))
	elseif idepth < 10000 then
		add(3 .. string.sub(depth, 1, 1))
		add(2 .. string.sub(depth, 2, 2))
		add(1 .. string.sub(depth, 3, 3))
		add(7 .. string.sub(depth, 4, 4))
	elseif idepth < 100000 then
		add(4 .. string.sub(depth, 1, 1))
		add(3 .. string.sub(depth, 2, 2))
		add(2 .. string.sub(depth, 3, 3))
		add(1 .. string.sub(depth, 4, 4))
		add(7 .. string.sub(depth, 5, 5))
	end

	Debug.StopPerformance('Lua Code - SAFCON01')
	return instructions
end
