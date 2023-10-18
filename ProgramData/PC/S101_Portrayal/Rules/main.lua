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
		Debug.StartPerformance('Lua Code - Dataset processing')

		local feature = featurePortrayalItem.Feature

		--Debug.Break()

		local featurePortrayal = featurePortrayalItem:NewFeaturePortrayal()

		contextParameters._observed = {}
		
		local status, err = pcall(function ()
			Debug.StartPerformance('Lua Code - Rules processing')

			local dateDependent = ProcessFixedAndPeriodicDates(feature, featurePortrayal)

			local scaleMinimum = feature['!scaleMinimum']
			local scaleMaximum = feature['!scaleMaximum']

			if scaleMinimum and not contextParameters.IgnoreScamin then
				featurePortrayal:AddInstructions('ScaleMinimum:' .. scaleMinimum)
			end

			if scaleMaximum then
				featurePortrayal:AddInstructions('ScaleMaximum:' .. scaleMaximum)
			end

			require(feature.Code)
			local viewingGroup = _G[feature.Code](feature, featurePortrayal, contextParameters)

			if not viewingGroup then
				error('Viewing group not returned')
			end

			if #featurePortrayal.DrawingInstructions == 0 then
				error('No drawing instructions were emitted for feature ' .. feature.ID)
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

		--Debug.Break()

		local observed = ObservedContextParametersAsString(featurePortrayalItem)

		local drawingInstructions = table.concat(featurePortrayal.DrawingInstructions, ';')

		Debug.StopPerformance('Lua Code - HostPortrayalEmit preparation')

		return HostPortrayalEmit(featurePortrayal.FeatureReference, drawingInstructions, observed)
	end

	--Debug.Break()

	if featureIDs then
		for _,  featureID in ipairs(featureIDs) do
			if not ProcessFeaturePortrayalItem(featurePortrayalItems[featureID]) then
				return false
			end
		end
	else
		for _,  featurePortrayalItem in ipairs(featurePortrayalItems) do
			if not ProcessFeaturePortrayalItem(featurePortrayalItem) then
				return false
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
		-- Never called when o1 and o2 are the same table.
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

sqParams = {'SpatialAssociation', 'defines', 'SpatialQuality'}
