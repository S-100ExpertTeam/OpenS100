--[[
This file contains the global functions that define the Lua Portrayal Model classes.
These functions are intended to be called by the portrayal rules.
--]]
-- #61

require 'PortrayalAPI'

local CreateContextParameters, CreateFeaturePortrayalItemArray, CreateFeaturePortrayal, CreateDrawingInstructions
local GetMergedDisplayParameters

PortrayalModel =
{
	Type = 'PortrayalModel'
}

function PortrayalModel.CreatePortrayalContext()
	local portrayalContext =
	{
		Type = 'PortrayalContext',
		ContextParameters = CreateContextParameters(),
		FeaturePortrayalItems = CreateFeaturePortrayalItemArray()
	}

	Debug.StopPerformance('Lua Code - Total')
	local featureIDs = HostGetFeatureIDs()
	Debug.StartPerformance('Lua Code - Total')

	for _, featureID in ipairs(featureIDs) do
		Debug.StopPerformance('Lua Code - Total')
		local featureCode = HostFeatureGetCode(featureID)
		Debug.StartPerformance('Lua Code - Total')
		local feature = CreateFeature(featureID, featureCode)

		portrayalContext.FeaturePortrayalItems:AddFeature(feature)
	end

	function portrayalContext:GetFeatures(featureType)
		local features = {}

		for _, featurePortrayalItem in ipairs(self.FeaturePortrayalItems) do
			local feature = featurePortrayalItem.Feature

			if featureType == nil or feature.Code == featureType then
				features[#features + 1] = feature
			end
		end

		return features
	end

	return portrayalContext
end

function ObservedContextParametersAsString(featurePortrayalItem)
	Debug.StartPerformance('Lua Code - ObservedContextParametersAsString')

	local inUse = featurePortrayalItem.InUseContextParameters

	local observedValues = {}

	for observed, _ in pairs(featurePortrayalItem.ObservedContextParameters) do
		if observed ~= "_observed" then
			local value = inUse[observed]

			if type(value) == 'boolean' then
				value = value and 'true' or 'false'
			elseif type(value) == 'table' and value.Type == 'ScaledDecimal' then
				value = value:ToNumber()
			end

			observedValues[#observedValues + 1] = observed .. ':' .. value;
		end
	end

	observedValues = table.concat(observedValues, ';')

	Debug.StopPerformance('Lua Code - ObservedContextParametersAsString')

	return observedValues
end

function CreateContextParameters()
	local contextParameters = { _observed = {} }
	
	local ppMetaTable =
	{
		__index = function (t, k)
			if k == '_asTable' then
				local cp = {}

				for k, v in pairs(contextParameters) do
					cp[k] = v
				end

				return cp
			elseif k == '_underlyingTable' then
				return contextParameters
			else
				local r = contextParameters[k]
			
				if r == nil then
					error('Invalid mariner setting "' .. tostring(k) .. '"', 2)
				end
			
				contextParameters._observed[k] = true

				--Debug.Trace('Portrayal parameter "' .. k .. '" observed.')

				return r;
			end
		end,
		
		__newindex = function (t, k, v)
			if contextParameters[k] == nil then
				error('Attempt to set invalid portrayal parameter "' .. tostring(k) .. '"', 2)
			end
			
			contextParameters[k] = v

			if type(v) == 'boolean' then
				-- Cannot concatenate booleans
				if v then
					Debug.Trace('Setting portrayal parameter: ' .. k .. ' = true')
				else
					Debug.Trace('Setting portrayal parameter: ' .. k .. ' = false')
				end
			elseif type(v) ~= 'table' then
				Debug.Trace('Setting portrayal parameter: ' .. k .. ' = ' .. v .. '')
			elseif v.Type == 'ScaledDecimal' then
				Debug.Trace('Setting portrayal parameter: ' .. k .. ' = ' .. v:ToNumber() .. '')
			end
		end
	}
	
	local ppProxy = { Type = 'ContextParametersProxy' }
	
	setmetatable(ppProxy, ppMetaTable)
	
	return ppProxy
end

function CreateFeaturePortrayalItemArray()
	local featurePortrayalItemArray = { Type = 'array:FeaturePortrayalItem' }

	function featurePortrayalItemArray:AddFeature(feature)
		CheckType(feature, 'Feature')

		local featurePortrayalItem = { Type = 'FeaturePortrayalItem', Feature = feature, ObservedContextParameters = {} }

		-- Note: variables created in a feature should start with an underscore to prevent name collisions with S-100 attribute names
		feature._featurePortrayalItem = featurePortrayalItem

		function featurePortrayalItem:NewFeaturePortrayal()
			-- Note:
			--   The naming pattern for S100_FC_Item is "[A-Za-z][A-Za-z0-9_]*" (upper or lower case letter followed by zero or more letters, numbers, or underscores).
			--   When adding information to the feature class, use variable names that won't cause name collisions with attribute codes.
			self.Feature._featurePortrayal = CreateFeaturePortrayal(self.Feature)

			return self.Feature._featurePortrayal
		end

		self[#self + 1] = featurePortrayalItem
		self[feature.ID] = featurePortrayalItem
	end

	return featurePortrayalItemArray
end

function InstructionSpatialReference(spatialAssociation)
	return spatialAssociation.SpatialID .. ',' .. spatialAssociation.Orientation.Name
end

function CreateFeaturePortrayal(feature)
	CheckType(feature, 'Feature')

	local featurePortrayal =
	{
		Type = 'FeaturePortrayal',
		Feature = feature,
		FeatureReference = feature.ID,
		DrawingInstructions = CreateDrawingInstructions(),
		GetFeatureNameCalled = false,
	}

	function featurePortrayal:AddInstructions(instructions)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(instructions, 'string')

		self.DrawingInstructions:Add(instructions)
	end
	
	-- Count the occurances of a given drawing instruction
	function featurePortrayal:GetInstructionCount(drawingInstruction)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(drawingInstruction, 'string')
		
		local count = 0
		for _, v in ipairs(self.DrawingInstructions) do
			for instruction in string.gmatch(v, "([^;]+)") do
				if string.find(instruction, drawingInstruction, 1, true) ~= nil then
					count = count + 1
				end
			end
		end
		return count
	end

	-- Count the number of text instructions which have been output by co-located features. This should only be called for point features.
	function featurePortrayal:GetColocatedTextCount()
		CheckType(self, featurePortrayal.Type)

		local textOffsetLines = 0
		local spatialAssociation = self.Feature:GetSpatialAssociation()

		for i, af in ipairs(spatialAssociation:GetAssociatedFeatures()) do
			if self.Feature.ID ~= af.ID then
				local afp = rawget(af, '_featurePortrayal')
				if afp ~= nil then
					textOffsetLines = textOffsetLines + afp:GetInstructionCount('TextInstruction')
				end
			end
		end
		
		return textOffsetLines
	end

	function featurePortrayal:AddTextInstruction(text, textViewingGroup, textPriority, viewingGroup, priority, isLightDescription)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(text, 'string')
		CheckType(textViewingGroup, 'number')
		CheckType(textPriority, 'number')
		CheckType(viewingGroup, 'number')
		CheckTypeOrNil(priority, 'number')
		
		if not text or text == '' then
			return
		end
		
		local function GetDrawingInstructions(text, textViewingGroup, textPriority)
			return 'ViewingGroup:' .. textViewingGroup .. ',' .. viewingGroup .. ';DrawingPriority:' .. textPriority .. ';TextInstruction:' .. text
		end
		
		local placementFeature = self.Feature
		
		local textAssociation = self.Feature:GetFeatureAssociations('TextAssociation')
		if not textAssociation or #textAssociation < 1 then
			-- Look for structure -> equipment -> TextPlacement
			local equipment = self.Feature:GetFeatureAssociations('StructureEquipment', 'theEquipment')
			if equipment then
				for _, feature in ipairs(equipment) do
					local t = feature:GetFeatureAssociations('TextAssociation')
					if t and #t > 0 then
						-- only use it to position our name if it isn't positioning the feature name on the equipment
						if (not feature['!featureName'] or #feature.featureName == 0 or not feature.featureName[1].name) and contains(1, t[1].textType) then
							textAssociation = t
							break
						end
					end
				end
			end
		end
		
		if textAssociation and #textAssociation > 0 then
		
			local textPlacementFeature = textAssociation[1]
			if not rawget(textPlacementFeature, '_initialized') then
				textPlacementFeature._initialized = true
				textPlacementFeature._processedFeatureName = false
				textPlacementFeature._processedLightName = false
			end

			-- 1: place feature name
			-- 2: place feature and/or light description
			local placementType = textPlacementFeature.textType or {}
			
			local isNewPortrayal = not rawget(textPlacementFeature, '_featurePortrayal')
			local hasFeatureName = rawget(textPlacementFeature, '_processedFeatureName')

			local isName = not isLightDescription and not hasFeatureName and self.GetFeatureNameCalled
			if isName then
				textPlacementFeature._processedFeatureName = true
			end
			
			local isNameOverride = isName
			local isNamePlacement = contains(1, placementType) and isName
			local isCharacterPlacement = contains(2, placementType) and not isLightDescription and not isName
			local isLightPlacement = contains(2, placementType) and isLightDescription
			
			if isNameOverride or isLightPlacement or isNamePlacement or isCharacterPlacement then
			
				local drawingInstructions = CreateDrawingInstructions()
				
				-- Make the TextPlacement feature the target of our drawing instructions
				placementFeature = textPlacementFeature
				
				if isNewPortrayal then
					local newPortrayal = placementFeature._featurePortrayalItem:NewFeaturePortrayal()
					placementFeature._name = {}
					placementFeature._featureCharacteristics = {}
					placementFeature._lightCharacteristics = {}

					-- Add the instructions to offset the text relative to the location of the TextPlacement feature
					local length = placementFeature.textOffsetDistance or 0
					local direction = placementFeature.textOffsetBearing or 0
					if length ~= 0 then
						newPortrayal:AddInstructions('AugmentedRay:GeographicCRS,' .. direction .. ',PortrayalCRS,' .. length .. ';LinePlacement:Relative,1')
					end
					
					if placementFeature.textRotation then
						newPortrayal:AddInstructions('TextAlignHorizontal:Start;TextAlignVertical:Center;Rotation:GeographicCRS,' .. direction)
					else
						if length == 0 then
							-- Center the text on the point
							newPortrayal:AddInstructions('TextAlignHorizontal:Center;TextAlignVertical:Center')
						elseif direction >=   5 and direction <  85 then
							newPortrayal:AddInstructions('TextAlignHorizontal:Start;TextAlignVertical:Bottom')
						elseif direction >=  85 and direction <  95 then
							newPortrayal:AddInstructions('TextAlignHorizontal:Start;TextAlignVertical:Center')
						elseif direction >=  95 and direction < 175 then
							newPortrayal:AddInstructions('TextAlignHorizontal:Start;TextAlignVertical:Top')
						elseif direction >= 175 and direction < 185 then
							newPortrayal:AddInstructions('TextAlignHorizontal:Center;TextAlignVertical:Top')
						elseif direction >= 185 and direction < 265 then
							newPortrayal:AddInstructions('TextAlignHorizontal:End;TextAlignVertical:Top')
						elseif direction >= 175 and direction < 275 then
							newPortrayal:AddInstructions('TextAlignHorizontal:End;TextAlignVertical:Center')
						elseif direction >= 175 and direction < 355 then
							newPortrayal:AddInstructions('TextAlignHorizontal:End;TextAlignVertical:Bottom')
						else
							newPortrayal:AddInstructions('TextAlignHorizontal:Center;TextAlignVertical:Bottom')
						end
					end
				end
				
				local targetTable
				if isNameOverride or isNamePlacement then
					targetTable = placementFeature._name
				elseif isCharacterPlacement then
					targetTable = placementFeature._featureCharacteristics
				elseif isLightPlacement then
					targetTable = placementFeature._lightCharacteristics
				end
				
				if not portrayalContext.ContextParameters.IgnoreScaleMinimum then
					-- Add scaleMinimum if present
					local scaleMinimum = textPlacementFeature['!scaleMinimum']
					if scaleMinimum then
						-- Prefer value from text placement; it's guaranteed to turn off sooner
						drawingInstructions:Add('ScaleMinimum:' .. scaleMinimum)
					else
						scaleMinimum = self.Feature['!scaleMinimum']
						if scaleMinimum then
							-- Use value from source feature
							drawingInstructions:Add('ScaleMinimum:' .. scaleMinimum)
						end
					end
				end

				-- Copy relevant drawing instructions to the target feature (TextAlignHorizontal, TextAlignVertical, and TextVerticalOffset are intentionally not copied)
				local targetCommands =
				{
					['DisplayPlane:'] = 'DisplayPlane:UnderRadar',
					['FontColor:'] = 'FontColor:CHBLK',	-- transparency = 0
					['FontBackgroundColor:'] = "nil",	-- token="", transparency=1
					['FontSize:'] = "nil",				-- 10
					['FontProportion:'] = "nil",		-- "Proportional"
					['FontWeight:'] = "nil",			-- "Medium"
					['FontSlant:'] = "nil",				-- "Upright"
					['FontSerifs:'] = "nil",			-- false
					['FontUnderline:'] = "nil",			-- false
					['FontStrikethrough:'] = "nil",		-- false
					['FontUpperline:'] = "nil",			-- false
					['FontReference:'] = "nil",			-- ""
					['Hover:'] = "nil",					-- false
				}
				-- Store / Copy relevant time intervals
				local timeState = {}
				local timeCommands =
				{
					['Date:'] = "nil",
					['Time:'] = "nil",
					['DateTime:'] = "nil",
					['TimeValid:'] = "nil"
				}
				for _, v in ipairs(self.DrawingInstructions) do
					for instruction in string.gmatch(v, "([^;]+)") do
						if string.find(instruction, 'ClearTime', 1, true) ~= nil then
							-- ClearTime clears all accumulated time intervals
							timeState = {}
						end
						for k, _ in pairs(targetCommands) do
							if string.find(instruction, k, 1, true) ~= nil then
								-- Overwrite with current state
								targetCommands[k] = instruction
							end
						end
						for k, _ in pairs(timeCommands) do
							if string.find(instruction, k, 1, true) ~= nil then
								-- Accumulate state
								table.insert(timeState, instruction)
							end
						end
					end
				end
				
				-- Add the copied drawing instructions to the TextPlacement feature
				for k, v in pairs(targetCommands) do
					if v ~= "nil" then
						drawingInstructions:Add(v)
					end
				end
				-- Add the accumulated time intervals to the TextPlacement feature
				for k, v in ipairs(timeState) do
					drawingInstructions:Add(v)
				end
				-- Done copying drawing instructions

				-- Add observed context parameters
				-- Force addition of NationalLanguage
				local nationalLanguage = portrayalContext.ContextParameters.NationalLanguage
				-- TODO: merge the observed parameters with any existing
				placementFeature._featurePortrayalItem.ObservedContextParameters = portrayalContext.ContextParameters._observed
				placementFeature._featurePortrayalItem.InUseContextParameters = portrayalContext.ContextParameters._asTable
				
				-- Save the drawing instructions with the text
				drawingInstructions:Add(GetDrawingInstructions(text, textViewingGroup, textPriority))
				table.insert(targetTable, { text, drawingInstructions })
			end
		end
		
		if placementFeature == self.Feature then
			-- Add the instructions to draw the text
			placementFeature._featurePortrayal:AddInstructions(GetDrawingInstructions(text, textViewingGroup, textPriority))
			
			-- Reset the state in case the caller generates further non-text drawing instructions
			self:AddInstructions('ViewingGroup:' .. viewingGroup)
			if priority then
				self:AddInstructions('DrawingPriority:' .. priority)
			end
		end
	end

	function featurePortrayal:AddSpatialReference(spatialAssociation)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(spatialAssociation, 'SpatialAssociation')

		if spatialAssociation.Orientation.Name == 'Forward' then
			self.DrawingInstructions:Add('SpatialReference:' .. EncodeDEFString(spatialAssociation.SpatialID))
		else
			self.DrawingInstructions:Add('SpatialReference:' .. EncodeDEFString(spatialAssociation.SpatialID) .. ',false')
		end
	end

	--
	-- S-52 PresLib Ed 4.0.3 Part I_Clean.pdf, page 42
	-- LC � Showline (simple linestyle).
	--
	function featurePortrayal:SimpleLineStyle(lineType, width, colour)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(lineType, 'string')
		CheckType(width, 'number')		-- S-52 PresLib Ed 4.0.3 Part I_Clean.pdf, page 42 i.e. width 2 == .64 mm
		CheckType(colour, 'string')

		if lineType == 'solid' then
			self.DrawingInstructions:Add('LineStyle:_simple_,,' .. width .. ',' .. colour)
		elseif lineType == 'dash' then
			self.DrawingInstructions:Add('Dash:0,3.6;' .. 'LineStyle:_simple_,5.4,' .. width .. ',' .. colour)
		elseif lineType == 'dot' then
			self.DrawingInstructions:Add('Dash:0,0.6;' .. 'LineStyle:_simple_,1.8,' .. width .. ',' .. colour)
		else
			error('Invalid lineType ' .. lineType)
		end

		self.DrawingInstructions:Add()
	end

	--
	-- Evaluates TextPlacement and featureName; returns the first name which matches the selected national language. If no match is found,
	-- returns the first entry marked as the default (nameUsage == 1). Otherwise returns nil.
	function featurePortrayal:GetFeatureName(feature, contextParameters)
		CheckSelf(self, featurePortrayal.Type)
		CheckType(feature, 'Feature')
		CheckType(contextParameters, 'array:ContextParameter')

		self.GetFeatureNameCalled = true
		
		-- text attribute was removed as of the 1.4.1 FC
		-- TextPlacement can override feature name
		--local textAssociation = feature:GetFeatureAssociations('TextAssociation')
		--if textAssociation and #textAssociation > 0 and textAssociation[1].text then
			--return textAssociation[1].text
		--end
		
		if not feature['!featureName'] or #feature.featureName == 0 or not feature.featureName[1].name then
			return nil
		end
		
		local defaultName			-- an entry with nameUsage == 1
		for cnt, featureName in ipairs(feature.featureName) do

			-- ensure a name is present and it's intended for chart display
			if featureName.name and featureName.nameUsage then
				local languageMatches = (featureName.language and featureName.language == contextParameters.NationalLanguage)

				-- check for default values which are used if we can't otherwise find a match...
				if featureName.nameUsage == 1 then
					if languageMatches then
						return featureName.name
					end
					-- only one entry is permitted to have nameUsage set to one
					defaultName = featureName.name
				elseif featureName.nameUsage == 2 and languageMatches then
					-- use the entry intended for chart display which matched the selected lanaguage
					return featureName.name
				end
			end
		end
		
		return defaultName
	end
	
	return featurePortrayal
end

function CreateDrawingInstructions()
	local drawingInstructions = { Type = 'array:DrawingInstruction' }

	function drawingInstructions:Add(instruction)
		self[#self + 1] = instruction
	end

	return drawingInstructions;
end

function GetInformationText(information, contextParameters)
	local defaultText

	for _, text in ipairs(information.information) do
		if text.text and text.text ~= '' then
			if text.language then
				if text.language == contextParameters.NationalLanguage then
					-- return the national language text
					defaultText = text.text
					break
				end
				if text.language == 'eng' or text.language == '' then
					-- default to english text
					defaultText = defaultText or text.text
				end
			else
				-- no language specified, assume eng
				defaultText = defaultText or text.text
			end
		end
	end

	return defaultText
end

function GetFeatureName(feature, contextParameters)
	return feature._featurePortrayal:GetFeatureName(feature, contextParameters)
end

--
-- The caller should not assume any drawing instructions are emitted; the state
-- of the text style commands may or may not be altered after this call.
--
-- textStyleInstructions example: 'LocalOffset:0,0;FontColor:CHBLK'
function PortrayFeatureName(feature, featurePortrayal, contextParameters, textViewingGroup, textPriority, viewingGroup, priority, textStyleInstructions)
	local name = featurePortrayal:GetFeatureName(feature, contextParameters)
	if name then
		local textStyle = textStyleInstructions or 'FontColor:CHBLK'
		featurePortrayal:AddInstructions(textStyle)
		featurePortrayal:AddTextInstruction(EncodeString(name, '%s'), textViewingGroup, textPriority, viewingGroup, priority)
	end
end