require 'S100Scripting'
require 'PortrayalModel'
require 'PortrayalAPI'
require 'Default'

-- Main entry point for portrayal
function PortrayalMain(featureIDs)
	Debug.StartPerformance('Lua Code - Total')

	if not portrayalContext then
		error('Host must call PortrayalInitializeContextParameters() before calling portrayal_main()')
	end

	local contextParameters = portrayalContext.ContextParameters

	local featurePortrayalItems = portrayalContext.FeaturePortrayalItems

	-- For curves and surfaces, perform line symbology on self and adjacent features so that
	-- higher priority edges suppress lower priority edges.

	function ProcessFeaturePortrayalItem(featurePortrayalItem)

		local feature = featurePortrayalItem.Feature

		Debug.StartPerformance('Lua Code - Dataset processing')
		
		local featurePortrayal = featurePortrayalItem:NewFeaturePortrayal()

		contextParameters._observed = {}
		
		local status, err = pcall(function ()
			Debug.StartPerformance('Lua Code - Rules processing')

			local dateDependent = ProcessFixedAndPeriodicDates(feature, featurePortrayal)

			local scaleMinimum = feature['!scaleMinimum']
			local scaleMaximum = feature['!scaleMaximum']

			if scaleMinimum and not contextParameters.IgnoreScaleMinimum then
				featurePortrayal:AddInstructions('ScaleMinimum:' .. scaleMinimum)
			end

			if scaleMaximum then
				featurePortrayal:AddInstructions('ScaleMaximum:' .. scaleMaximum)
			end

			require(feature.Code)
			local viewingGroup = _G[feature.Code](feature, featurePortrayal, contextParameters)

			if not viewingGroup then
				error('Viewing group not returned for feature ' .. feature.ID)
			end

			if #featurePortrayal.DrawingInstructions == 0 then
				error('No drawing instructions were emitted for feature ' .. feature.ID)
			end

			-- Ensure featureName is processed for those features whose rules don't process it directly.
			if not featurePortrayal.GetFeatureNameCalled then
				PortrayFeatureName(feature, featurePortrayal, contextParameters, 32, 24, viewingGroup, nil, 'TextAlignHorizontal:Center;TextAlignVertical:Top;LocalOffset:0,-3.51;FontColor:CHBLK')
			end

			ProcessNauticalInformation(feature, featurePortrayal, contextParameters, viewingGroup)

			if dateDependent then
				AddDateDependentSymbol(feature, featurePortrayal, contextParameters, viewingGroup)
			end

			Debug.StopPerformance('Lua Code - Rules processing')
		end)

		if not status then
			Debug.Trace('Error: ' .. err .. '.  Default symbology for ' .. feature.Code .. ' ID=' .. feature.ID .. ' returned.')

			-- Clear any drawing instructions created up to this point.
			featurePortrayal = featurePortrayalItem:NewFeaturePortrayal()

			local dateDependent = ProcessFixedAndPeriodicDates(feature, featurePortrayal)
			local viewingGroup = Default(feature, featurePortrayal, contextParameters)
			ProcessNauticalInformation(feature, featurePortrayal, contextParameters, viewingGroup)
			if dateDependent then
				AddDateDependentSymbol(feature, featurePortrayal, contextParameters, viewingGroup)
			end
		end

		Debug.StopPerformance('Lua Code - Dataset processing')

		Debug.StartPerformance('Lua Code - HostPortrayalEmit preparation')

		featurePortrayalItem.ObservedContextParameters = contextParameters._observed
		featurePortrayalItem.InUseContextParameters = contextParameters._asTable

		local observed = ObservedContextParametersAsString(featurePortrayalItem)

		local drawingInstructions = table.concat(featurePortrayal.DrawingInstructions, ';')

		Debug.StopPerformance('Lua Code - HostPortrayalEmit preparation')

		return HostPortrayalEmit(featurePortrayal.FeatureReference, drawingInstructions, observed)
	end
	
	local textPlacementFeatures = {}
	
	if featureIDs then
		for _,  featureID in ipairs(featureIDs) do
			local item = featurePortrayalItems[featureID]
			-- TextPlacement gets processed when the feature it's associated with is processed
			if item.Feature.Code ~= 'TextPlacement' then
				if not ProcessFeaturePortrayalItem(item) then
					return false
				end
			else
				table.insert(textPlacementFeatures, item.Feature)
			end
		end
	else
		for _,  featurePortrayalItem in ipairs(featurePortrayalItems) do
			-- TextPlacement gets processed when the feature it's associated with is processed
			if featurePortrayalItem.Feature.Code ~= 'TextPlacement' then
				if not ProcessFeaturePortrayalItem(featurePortrayalItem) then
					return false
				end
			else
				table.insert(textPlacementFeatures, featurePortrayalItem.Feature)
			end
		end
	end
	
	-- Emit TextPlacement features
	require('TextPlacement')
	for _, feature in ipairs(textPlacementFeatures) do
		local featurePortrayal = rawget(feature, '_featurePortrayal')
		if featurePortrayal then
			TextPlacement(feature, featurePortrayal, contextParameters)	
			if featurePortrayal.DrawingInstructions then
				local item = rawget(feature, '_featurePortrayalItem')
				HostPortrayalEmit(featurePortrayal.FeatureReference, table.concat(featurePortrayal.DrawingInstructions, ';'), ObservedContextParametersAsString(item))
			else
				Debug.Trace('Warning: TextPlacement ID=' .. feature.ID .. ' has no drawing instructions.')
			end
		end
	end

	Debug.StopPerformance('Lua Code - Total')

	return true
end

-- Unknown value

local unknownValueMetatable =
{
	__eq = function (o1, o2)
		-- always false because reference equality is handled prior to the metamethod being called.
		-- this also means we don't have to worry about the EqMetaMethodGuarantee
		return false
	end,

	__lt = function (o1, o2)
		return false
	end,

	__le = function (o1, o2)
		return o1 == o2
	end
}

unknownValue = { Type = 'UnknownValue' }

setmetatable(unknownValue, unknownValueMetatable)

nilMarker = {}

scaminInfinite = 2147483647

sqParams = {'SpatialAssociation', 'theQualityInformation', 'SpatialQuality'}

-- Compatibility Checks

-- Later versions of Lua use table.unpack instead of a global function
unpack = unpack or table.unpack

-- Lua 5.3 and later evaluate the __eq metamethod differently than earlier versions. In earlier
-- versions the __eq metamethod is only called when both the lhs and rhs are the same type and
-- have the same metamethod.
--
-- In version 5.3 and later Lua looks for an __eq metamethod on the lhs and then on the rhs; if
-- any metamethod was found, it is called. There is no guarantee that the lhs and rhs are the same
-- type.
--
-- NOTE: S-100 requires a Lua 5.1 scripting engine. Support for other versions should not be taken
-- to imply that later versions will be supported in other cases. In cases where it is not possible
-- to support multiple versions (either due to technical challenges or resource constraints) the
-- scripts are only guaranteed to run as intended on Lua 5.1.
--
-- Lua 5.1: ScaledDecimal == unknownValue => false (because the __eq metamethods differ)
--
-- Lua 5.3: ScaledDecimal == unknownValue => error (because the ScaledDecimal __eq metamethod is called)
-- Lua 5.3: unknownValue == ScaledDecimal => false (because the unknownValue __eq metamethod is called)
EqMetaMethodGuarantee = true

local t1 = {}
local t2 = {}
local m1 = {}
local m2 = {}
setmetatable(t1, m1)
setmetatable(t2, m2)
m1.__eq = function(p1, p2) return true end
EqMetaMethodGuarantee = not (t1 == t2)
if not EqMetaMethodGuarantee then
	Debug.Trace('Warning: Non-standard Lua processor detected.')
end