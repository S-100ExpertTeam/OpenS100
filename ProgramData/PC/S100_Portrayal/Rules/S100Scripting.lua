--[[
This file contains the global functions and tables that define the S-100 Lua Scripting Model.
These functions are intended to be called by the S-100 scripts.
--]]
-- #80 - modularize processing of fixed and periodic date ranges
-- #119
-- #207

local orig_error = error

function error(message, depth)
	Debug.FirstChanceError(message, depth)

	orig_error(message, depth)
end

function EncodeString(str, fmt)
	if str == nil then
		return ''
	elseif fmt then
		if type(str) == 'table' and str.Type == 'ScaledDecimal' then
			str = str:ToNumber()
		end

		str = string.format(fmt, str)
	else
		str = tostring(str)
	end
	
	return EncodeDEFString(str)
end

--
-- Date/Time commands support
--

function ProcessPeriodicDateRanges(featurePortrayal, periodicDateRanges)
	local dateDependent = false

	if periodicDateRanges and #periodicDateRanges > 0 then
		for _, periodicDateRange in ipairs(periodicDateRanges) do
			local dateStart = periodicDateRange.dateStart
			local dateEnd = periodicDateRange.dateEnd

			featurePortrayal:AddInstructions('Date:' .. dateStart .. ',' .. dateEnd .. ';TimeValid:closedInterval')
		end

		dateDependent = true
	end

	return dateDependent
end

function ProcessFixedDateRange(featurePortrayal, fixedDateRange)
	local dateDependent = false
	
	if fixedDateRange then
		local dateStart = fixedDateRange.dateStart
		local dateEnd = fixedDateRange.dateEnd

		if dateStart and dateEnd then
			featurePortrayal:AddInstructions('Date:' .. dateStart .. ',' .. dateEnd .. ';TimeValid:closedInterval')
		elseif dateStart then
			featurePortrayal:AddInstructions('Date:' .. dateStart .. ';TimeValid:geSemiInterval')
		elseif dateEnd then
			featurePortrayal:AddInstructions('Date:,' .. dateEnd .. ';TimeValid:leSemiInterval')
		end

		dateDependent = true
	end
	
	return dateDependent
end

function ProcessFixedAndPeriodicDates(feature, featurePortrayal)
	local periodicDependent = ProcessPeriodicDateRanges(featurePortrayal, feature['!periodicDateRange'])
	local fixedDependent = ProcessFixedDateRange(featurePortrayal, feature['!fixedDateRange'])
	
	return periodicDependent or fixedDependent
end

function AddDateDependentSymbol(feature, featurePortrayal, contextParameters, viewingGroup)
	-- Clear any existing transforms and geometries
	featurePortrayal:AddInstructions('LocalOffset:0,0;LinePlacement:Relative,0.5;AreaPlacement:VisibleParts;AreaCRS:GlobalGeometry;Rotation:PortrayalCRS,0;ScaleFactor:1;ClearGeometry')

	featurePortrayal:AddInstructions('Hover:true')

	local displayPlane = contextParameters.RadarOverlay and 'DisplayPlane:OverRADAR' or 'DisplayPlane:UnderRADAR'

	featurePortrayal:AddInstructions(displayPlane)
	featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',31032,highlightDateDependent;DrawingPriority:24;PointInstruction:CHDATD01')
end

--
-- Nautical information support
--

function ProcessNauticalInformation(feature, featurePortrayal, contextParameters, viewingGroup)
	local function GetViewingGroups(container, vg31030, vg31031)
		if container then
			if container['!pictorialRepresentation'] then
				vg31031 = true
			end

			if container['!information'] then
				for _, information in ipairs(container.information) do
					if information.text then
						vg31030 = true
					end

					if information.fileReference then
						vg31031 = true
					end
				end
			end

			if container['!shapeInformation'] and next(container.shapeInformation) then
				vg31030 = true
			end

			if container['!topmark'] and container.topmark.shapeInformation and next(container.topmark.shapeInformation) then
				vg31030 = true
			end
		end

		return vg31030, vg31031
	end

	local vg31030, vg31031

	vg31030, vg31031 = GetViewingGroups(feature, vg31030, vg31031)
	vg31030, vg31031 = GetViewingGroups(feature:GetInformationAssociation('AdditionalInformation', 'providesInformation', 'NauticalInformation'), vg31030, vg31031)
	vg31030, vg31031 = GetViewingGroups(feature:GetInformationAssociation('AdditionalInformation', 'providesInformation', 'NonStandardWorkingDay'), vg31030, vg31031)
	vg31030, vg31031 = GetViewingGroups(feature:GetInformationAssociation('AdditionalInformation', 'providesInformation', 'ServiceHours'), vg31030, vg31031)

	if vg31030 or vg31031 then
		-- Clear any existing transforms and geometries
		featurePortrayal:AddInstructions('LocalOffset:0,0;LinePlacement:Relative,0.5;AreaPlacement:VisibleParts;AreaCRS:GlobalGeometry;Rotation:PortrayalCRS,0;ScaleFactor:1;ClearGeometry')

		featurePortrayal:AddInstructions('Hover:true')

		local displayPlane = contextParameters.RadarOverlay and 'DisplayPlane:OverRADAR' or 'DisplayPlane:UnderRADAR'

		if vg31030 then
			featurePortrayal:AddInstructions(displayPlane)
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',31030,highlightInfo;DrawingPriority:24;PointInstruction:INFORM01')
		end

		if vg31031 then
			featurePortrayal:AddInstructions(displayPlane)
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ',31031,highlightDocument;DrawingPriority:24;PointInstruction:INFORM01')
		end
	end
end

--
-- Encoded attribute value type support
--

local valueTypes = { 'boolean', 'enumeration', 'integer', 'real', 'text', 'date', 'time' , 'dateTime' , 'URI' , 'URL' , 'URN' , 'S100_CodeList' , 'S100_TruncatedDate' }

function ConvertEncodedValue(valueType, value)
	if value == unknownAttributeValueString then
		return unknownValue
	end

	if not contains(valueType, valueTypes) then
		error('Invalid parameter type:' .. valueType)
	end

	if valueType == 'boolean' then
		return (value == '1') or (value == 'true')
	elseif valueType == 'enumeration' then
		return tonumber(value)
	elseif valueType == 'integer' then
		return tonumber(value)
	elseif valueType == 'real' then
		return StringToScaledDecimal(value)
	else
		return value
	end
end

--
-- Scaled decimal support
--

function StringToScaledDecimal(value)
	if value then
		local i = value:find('%.')

		if i == nil then
			return CreateScaledDecimal(tonumber(value), 0)
		else
			local a = value:sub(1, i - 1)
			value = value:sub(1, i - 1) .. value:sub(i + 1, #value)

			return CreateScaledDecimal(tonumber(value), #value + 1 - i)
		end
	end
end

function NormalizeScaledDecimals(scaledDecimal1, scaledDecimal2)
	local sd1 = scaledDecimal1
	local sd2 = scaledDecimal2

	if sd1.Scale > sd2.Scale then
		sd1, sd2 = sd2, sd1
	end

	for _ = sd1.Scale + 1, sd2.Scale do
		sd1.Value = sd1.Value * 10
		sd1.Scale = sd1.Scale + 1
	end
end

local function ScaledDecimalToNumber(scaledDecimal)
	CheckType(scaledDecimal, 'ScaledDecimal')

	local value = scaledDecimal.Value

	for _ = 1, scaledDecimal.Scale do
		value = value / 10
	end

	return value
end

function ScaledDecimalCompare(scaledDecimal1, scaledDecimal2)
	CheckType(scaledDecimal1, 'ScaledDecimal')
	CheckType(scaledDecimal2, 'ScaledDecimal')

	local sd1 = { Value = scaledDecimal1.Value, Scale = scaledDecimal1.Scale }
	local sd2 = { Value = scaledDecimal2.Value, Scale = scaledDecimal2.Scale }

	NormalizeScaledDecimals(sd1, sd2)

	return sd1.Value - sd2.Value
end

local function ScaledDecimalSplit(scaledDecimal)
	CheckType(scaledDecimal, 'ScaledDecimal')

	local sign
	local value

	if scaledDecimal.Value < 0 then
		sign = '-'
		value = tostring(-scaledDecimal.Value)
	else
		sign = '+'
		value = tostring(scaledDecimal.Value)
	end

	if scaledDecimal.Scale == 0 then
		return sign, value, ''
	end

	local scaleLeft = #value - scaledDecimal.Scale

	if scaleLeft <= 0 then
		local leading = string.rep('0', -scaleLeft)

		return sign, '0', leading .. value
	else
		local left = value:sub(1, scaleLeft)
		local right = value:sub(scaleLeft + 1, #value)

		return sign, left, right
	end
end

local scaledDecimalMetatable =
{
	__eq = function (lhs, rhs)
		return ScaledDecimalCompare(lhs, rhs) == 0
	end,

	__lt = function (lhs, rhs)
		return ScaledDecimalCompare(lhs, rhs) < 0
	end,

	__le = function (lhs, rhs)
		return ScaledDecimalCompare(lhs, rhs) <= 0
	end,

	__add = function (scaledDecimal1, scaledDecimal2)
		local sd1 = { Value = scaledDecimal1.Value, Scale = scaledDecimal1.Scale }
		local sd2 = { Value = scaledDecimal2.Value, Scale = scaledDecimal2.Scale }

		NormalizeScaledDecimals(sd1, sd2)

		return CreateScaledDecimal(sd1.Value + sd2.Value, sd1.Scale)
	end,

	__sub = function (scaledDecimal1, scaledDecimal2)
		local sd1 = { Value = scaledDecimal1.Value, Scale = scaledDecimal1.Scale }
		local sd2 = { Value = scaledDecimal2.Value, Scale = scaledDecimal2.Scale }

		NormalizeScaledDecimals(sd1, sd2)

		return CreateScaledDecimal(sd1.Value - sd2.Value, sd1.Scale)
	end,

	__mul = function (scaledDecimal1, scaledDecimal2)
		local sd1 = { Value = scaledDecimal1.Value, Scale = scaledDecimal1.Scale }
		local sd2 = { Value = scaledDecimal2.Value, Scale = scaledDecimal2.Scale }

		NormalizeScaledDecimals(sd1, sd2)

		return CreateScaledDecimal(sd1.Value * sd2.Value, sd1.Scale + sd2.Scale)
	end,

	__div = function (scaledDecimal1, scaledDecimal2)
	end,

	__tostring = function (t)
		if t.Scale == 0 then
			return tostring(t.Value)
		end

		local sign
		local value

		if t.Value < 0 then
			sign = '-'
			value = tostring(-t.Value)
		else
			sign = ''
			value = tostring(t.Value)
		end

		local scaleLeft = #value - t.Scale

		if scaleLeft <= 0 then
			local leading = string.rep('0', -scaleLeft)

			return sign .. '0.' .. leading .. value
		else
			local left = value:sub(1, scaleLeft)
			local right = value:sub(scaleLeft + 1, #value)

			return sign .. left .. '.' .. right
		end
	end
}

function CreateScaledDecimal(value, scale)
	scale = scale or 0

	CheckType(value, 'number')
	CheckType(scale, 'number')

	local scaledDecimal = { Type = 'ScaledDecimal', Value = value, Scale = scale,
		Split = ScaledDecimalSplit,
		ToNumber = ScaledDecimalToNumber }

	setmetatable(scaledDecimal, scaledDecimalMetatable)

	return scaledDecimal
end

local scaledDecimalsScale = {}

function SD(value, scale)
	scale = scale or 0

	local scaledDecimals = scaledDecimalsScale[scale]

	if not scaledDecimals then
		scaledDecimals = {}
		scaledDecimalsScale[scale] = scaledDecimals
	end

	local sd = scaledDecimals[value]

	if not sd then
		sd = CreateScaledDecimal(value, scale)

		scaledDecimals[value] = sd
	end
		
	return sd
end

--
-- Type system support
--

function DerivedType(derived)
	local derivedMetatable =
	{
		__index = function (t, k)
			local value = rawget(t, k)

			while value == nil and t.Base ~= nil do
				t = t.Base

				value = rawget(t, k)
			end

			return value
		end
	}

	setmetatable(derived, derivedMetatable)

	return derived
end

local typeSystemChecksEnabled

function TypeSystemChecks(enabled)
	if typeSystemChecksEnabled == enabled then
		return false
	end

	typeSystemChecksEnabled = enabled

	if enabled then
		function CheckSelf(object, typeName, errorDepth)
			local objectType = type(object)
	
			if objectType == 'table' and object.Type == typeName then
				return
			end

			error('Function call on object of type ' .. typeName .. ' was not in the form of "object:function()"', errorDepth or 3)
		end

		function CheckNotSelf(object, typeName, errorDepth)
			local objectType = type(object)
	
			if objectType == 'table' and object.Type == typeName then
				error('Function call on object of type ' .. typeName .. ' was not in the form of "object.function()"', errorDepth or 3)
			end
		end

		function CheckType(object, typeName, errorDepth)
			local objectType = type(object)
			local colon = string.find(typeName, ':')

			if objectType == 'table' and colon then
				local highType = string.sub(typeName, 1, colon - 1)
				typeName = string.sub(typeName, colon + 1)

				if highType ~= 'array' and highType ~= 'array+' then
					error('Unrecognized high type "' .. highType '"', errorDepth or 3)
				end

				-- 'array' allows for empty arrays, 'array+' doesn't.
				if #object ~= 0 or highType == 'array+' then
					CheckType(object[1], typeName, (errorDepth or 3) + 1)
				end
			elseif objectType ~= 'table' then
				if objectType ~= typeName then
					error('Object given is of type "' .. objectType .. '" not of type "' .. typeName .. '"', errorDepth or 3)
				end
			elseif object == nil or object.Type ~= typeName then
				error('Object given is of type "' .. objectType .. '" not of type "' .. typeName .. '"', errorDepth or 3)
			end
		end

		function CheckTypeOrNil(object, typeName, errorDepth)
			if object ~= nil then
				errorDepth = errorDepth or 3

				CheckType(object, typeName, errorDepth + 1)
			end
		end

		-- Replace when inheritance is implemented.
		function CheckTypeOneOf(object, ...)
			if type(object) ~= 'table' then
				error('Object must be of table type.', 3)
			end

			for _, typeName in ipairs{...} do
				if object.Type == typeName then
					return
				end
			end

			error('Object not of any of the given types.', 3)
		end

		-- Replace when inheritance is implemented.
		function CheckTypeOneOfOrNil(object, ...)
			if object ~= nil then
				if type(object) ~= 'table' then
					error('Object must be of table type.', 3)
				end

				for _, typeName in ipairs{...} do
					if object.Type == typeName then
						return
					end
				end

				error('Object not of any of the given types.', 3)
			end
		end
	else
		function CheckSelf(...)
		end

		function CheckNotSelf(...)
		end

		function CheckType(...)
		end

		function CheckTypeOrNil(...)
		end

		function CheckTypeOneOf(...)
		end

		function CheckTypeOneOfOrNil(...)
		end
	end

	return true
end

-- Disabled by default
TypeSystemChecks(false)

typeInfo = nil

function GetTypeInfo()
	if not typeInfo then
		local ti = {}
		Debug.StopPerformance('Lua Code - Total')
		local featureTypeCodes = HostGetFeatureTypeCodes()
		local informationTypeCodes = HostGetInformationTypeCodes()
		local simpleAttributeCodes = HostGetSimpleAttributeTypeCodes()
		local complexAttributeCodes = HostGetComplexAttributeTypeCodes()
		local roleCodes = HostGetRoleTypeCodes()
		local informationAssociationCodes = HostGetInformationAssociationTypeCodes()
		local featureAssociationCodes = HostGetFeatureAssociationTypeCodes()
		
		Debug.StartPerformance('Lua Code - Total')

		ti.FeatureTypeInfos = {}

		for _, code in ipairs(featureTypeCodes or {}) do
			ti.FeatureTypeInfos[code] = { Type = 'FeatureTypeInfo', Code = code }
		end

		ti.InformationTypeInfos = {}

		for _, code in ipairs(informationTypeCodes or {}) do
			ti.InformationTypeInfos[code] = { Type = 'InformationTypeInfo', Code = code }
		end

		ti.SimpleAttributeInfos = {}

		for _, code in ipairs(simpleAttributeCodes or {}) do
			ti.SimpleAttributeInfos[code] = { Type = 'SimpleAttributeInfo', Code = code }
		end

		ti.ComplexAttributeInfos = {}

		for _, code in ipairs(complexAttributeCodes or {}) do
			ti.ComplexAttributeInfos[code] = { Type = 'ComplexAttributeInfo', Code = code }
		end

		ti.RoleInfos = {}

		for _, code in ipairs(roleCodes or {}) do
			ti.RoleInfos[code] = { Type = 'RoleInfo', Code = code }
		end

		ti.InformationAssociationInfos = {}

		for _, code in ipairs(informationAssociationCodes or {}) do
			ti.InformationAssociationInfos[code] = { Type = 'InformationAssociationInfo', Code = code }
		end

		ti.FeatureAssociationInfos = {}

		for _, code in ipairs(featureAssociationCodes or {}) do
			ti.FeatureAssociationInfos[code] = { Type = 'FeatureAssociationInfo', Code = code }
		end

		typeInfo = ti
	end

	return typeInfo
end

function GetFeatureTypeInfo(code)
	local typeInfo = GetTypeInfo()

	if not typeInfo.FeatureTypeInfos[code] then
		-- Try to process codes which don't exist in the FC
		-- error('Invalid feature code')
		return nil
	end

	if not typeInfo.FeatureTypeInfos[code].TypeInfo then
		Debug.StopPerformance('Lua Code - Total')
		typeInfo.FeatureTypeInfos[code].TypeInfo = HostGetFeatureTypeInfo(code)
		Debug.StartPerformance('Lua Code - Total')
	end

	return typeInfo.FeatureTypeInfos[code].TypeInfo
end

function GetInformationTypeInfo(code)
	local typeInfo = GetTypeInfo()

	if not typeInfo.InformationTypeInfos[code] then
		error('Invalid information type code')
	end

	if not typeInfo.InformationTypeInfos[code].TypeInfo then
		Debug.StopPerformance('Lua Code - Total')
		typeInfo.InformationTypeInfos[code].TypeInfo = HostGetInformationTypeInfo(code)
		Debug.StartPerformance('Lua Code - Total')
	end

	return typeInfo.InformationTypeInfos[code].TypeInfo
end

function GetSimpleAttributeTypeInfo(code)
	local typeInfo = GetTypeInfo()

	if not typeInfo.SimpleAttributeInfos[code] then
		error('Invalid information type code')
	end

	if not typeInfo.SimpleAttributeInfos[code].TypeInfo then
		Debug.StopPerformance('Lua Code - Total')
		typeInfo.SimpleAttributeInfos[code].TypeInfo = HostGetSimpleAttributeTypeInfo(code)
		Debug.StartPerformance('Lua Code - Total')
	end

	return typeInfo.SimpleAttributeInfos[code].TypeInfo
end

function GetComplexAttributeTypeInfo(code)
	local typeInfo = GetTypeInfo()

	if not typeInfo.ComplexAttributeInfos[code] then
		error('Invalid information type code')
	end

	if not typeInfo.ComplexAttributeInfos[code].TypeInfo then
		Debug.StopPerformance('Lua Code - Total')
		typeInfo.ComplexAttributeInfos[code].TypeInfo = HostGetComplexAttributeTypeInfo(code)
		Debug.StartPerformance('Lua Code - Total')
	end

	return typeInfo.ComplexAttributeInfos[code].TypeInfo
end

unknownAttributeValueString = '13BD40516CF742E886D5B4125DBB89742A043D0050E44B568CBB1FDDA8B464FF'

--
-- Helper functions
--

function contains(value, array)
	for i = 1, #array do
		if array[i] == value then
			return true
		end
	end
	
	if type(value) == 'table' then
		for i = 1, #array do
			for j = 1, #value do
				if array[i] == value[j] then
					return true
				end
			end
		end
	end

	return false
end

--
-- Debugging hooks back into host
--

Debug = {}

function Debug.Break()
	if HostDebuggerEntry then
		HostDebuggerEntry('break')
	end
end

function Debug.Trace(message)
	if HostDebuggerEntry then
		HostDebuggerEntry('trace', message)
	end
end

function Debug.StartPerformance(name)
	if HostDebuggerEntry then
		HostDebuggerEntry('start_performance', name)
	end
end

function Debug.StopPerformance(name)
	if HostDebuggerEntry then
		HostDebuggerEntry('stop_performance', name)
	end
end

function Debug.ResetPerformance(name)
	if HostDebuggerEntry then
		HostDebuggerEntry('reset_performance', name)
	end
end

function Debug.FirstChanceError(message, depth)
	if HostDebuggerEntry then
		HostDebuggerEntry('first_chance_error', message, depth)
	end
end

--
-- Preinitialized scaled decimals
--

scaledDecimalZero = CreateScaledDecimal(0, 0)
scaledDecimalOne = CreateScaledDecimal(1, 0)

--
-- Unit tests
--

function RunUnitTests()
	local scaledDecimals        = {  '.01', '00.001', '42', '1.0', '1',  '.12010', '20.13', '0.103', '0.11',  '.1', '0.1', '0.10', '00.1', '-23', '-0.1', '-2.2',  '-.1',  '-.0010', '-1.002' }
	local scaledDecimalsAnswers = { '0.01',  '0.001', '42', '1.0', '1', '0.12010', '20.13', '0.103', '0.11', '0.1', '0.1', '0.10',  '0.1', '-23', '-0.1', '-2.2', '-0.1', '-0.0010', '-1.002' }

	local wholePartSigns        = {    '+',     '+',   '+',   '+', '+',        '+',     '+',    '+',    '+',   '+',   '+',    '+',    '+',   '-',    '-',    '-',    '-',       '-',      '-' }
	local wholeParts            = {    '0',     '0',  '42',   '1', '1',        '0',    '20',    '0',    '0',   '0',   '0',    '0',    '0',  '23',    '0',    '2',    '0',       '0',      '1' }
	local fractionalParts       = {   '01',   '001',    '',   '0',  '',    '12010',    '13',  '103',   '11',   '1',   '1',   '10',    '1',    '',    '1',    '2',    '1',     '0010',   '002' }

	for i, v in ipairs(scaledDecimals) do
		local sd = StringToScaledDecimal(v)
		local sds = tostring(sd)

		if sds ~= scaledDecimalsAnswers[i] then
			return 'Scaled decimal test failed for "' .. v .. '" round trip returned "' .. sds .. '" expected "' .. scaledDecimalsAnswers[i] .. '"'
		end

		local sign, whole, fractional = ScaledDecimalSplit(sd)

		if sign ~= wholePartSigns[i] or whole ~= wholeParts[i] or fractional ~= fractionalParts[i] then
			Debug.Break()
			return 'Scaled decimal test failed for "' .. v .. '" split returned "' .. sign .. ',' .. whole .. ',' .. fractional .. '" expected "' .. wholePartSigns[i] .. ',' .. wholeParts[i] .. ',' .. fractionalParts[i] .. '"'
		end
	end

	local operand1        = { '1', '1.0', '-1',   '0.002', '0',  '3',  '-6',  '-7'  }
	local operand2        = { '2', '2',   '2.2',  '3',     '-1', '7',  '5',   '-8'  }
	local addTestsAnswers = { '3', '3',   '1.2',  '3.002', '-1', '10', '-1',  '-15' }
	local mulTestsAnswers = { '2', '2',   '-2.2', '0.006', '0',  '21', '-30', '56'  }

	for i, v in ipairs(operand1) do
		local o1 = StringToScaledDecimal(operand1[i])
		local o2 = StringToScaledDecimal(operand2[i])

		local ar = o1 + o2
		local mr = o1 * o2

		local ae = StringToScaledDecimal(addTestsAnswers[i])
		local me = StringToScaledDecimal(mulTestsAnswers[i])

		if ar ~= ae then
			Debug.Break()
			return 'Scaled decimal add test failed for ' .. operand1[i] .. '+' .. operand2[i] .. ' expected ' .. addTestsAnswers[i] .. ' got ' .. tostring(ar)
		end

		if mr ~= me then
			Debug.Break()
			return 'Scaled decimal multiply test failed for ' .. operand1[i] .. '+' .. operand2[i] .. ' expected ' .. mulTestsAnswers[i] .. ' got ' .. tostring(ar)
		end
	end

	local inputStrings   = { 'Hello, World!',  'Foo:bar',  'This & that',  'Emdedded &c colon code',  'Double ampersand &&',   'Mish mash: &e &&& &m, blah;',         'Normal Text' }
	local encodedStrings = { 'Hello&m World!', 'Foo&cbar', 'This &a that', 'Emdedded &ac colon code', 'Double ampersand &a&a', 'Mish mash&c &ae &a&a&a &am&m blah&s', 'Normal Text' }

	for i, inputString in ipairs(inputStrings) do
		local encodedString = encodedStrings[i]

		local result = EncodeDEFString(inputString)

		if result ~= encodedString then
			Debug.Break()
			return 'EncodeDEFString test failed for "' .. inputString .. '" expected "' .. encodedString .. '" got "' .. result .. '"'
		end

		result = DecodeDEFString(encodedString)

		if result ~= inputString then
			Debug.Break()
			return 'DecodeDEFString test failed for "' .. encodedString .. '" expected "' .. inputString .. '" got "' .. result .. '"'
		end
	end
end
