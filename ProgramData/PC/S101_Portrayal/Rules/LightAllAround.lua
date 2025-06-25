-- LightAllAround portrayal rules file.
-- #52
-- #60

-- Referenced CSPs.
require 'LightFlareAndDescription'
require 'LITDSN02'

-- Main entry point for feature type.
function LightAllAround(feature, featurePortrayal, contextParameters)
	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:OverRadar;Hover:true')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:UnderRadar;Hover:true')
	end

	local valueOfNominalRange = 9.0

	if feature.valueOfNominalRange then
		valueOfNominalRange = feature.valueOfNominalRange:ToNumber()
	end

	local categoryOfLight = feature.categoryOfLight or {}

	-- majorLight is not valid for aero and non morse code lights
	local notAeroLight = not contains(categoryOfLight, { 5 })
	local notMorseCodeLight = feature.rhythmOfLight == nil or feature.rhythmOfLight.lightCharacteristic ~= 12
	local majorLight = feature.majorLight and notAeroLight and notMorseCodeLight

	if contains(categoryOfLight, { 8, 11 }) then
		-- Flood or spot light
		featurePortrayal:AddInstructions('PointInstruction:LIGHTS82')
	elseif contains(categoryOfLight, { 9 }) then
		-- Strip light
		featurePortrayal:AddInstructions('PointInstruction:LIGHTS81')
	elseif majorLight then
		-- Non aero and non morse code light marked as a majorLight
		local colour = feature.colour
		local sectorColourToken

		if colour[1] == 1 and colour[2] == 3 then -- white and red
			sectorColourToken = 'LITRD'
		elseif colour[1] == 3 then -- red
			sectorColourToken = 'LITRD'
		elseif colour[1] == 1 and colour[2] == 4 then -- white and green
			sectorColourToken = 'LITGN'
		elseif colour[1] == 4 then -- green
			sectorColourToken = 'LITGN'
		elseif colour[1] == 11 or colour[1] == 6 or colour[1] == 1 then -- orange or yellow or white
			sectorColourToken = 'LITYW'
		elseif colour[1] == 5 and colour[2] == 6 then -- blue and yellow
			sectorColourToken = 'LITYW'
		else
			sectorColourToken = 'CHMGD'
		end

		featurePortrayal:AddInstructions('ArcByRadius:0,0,26,0,360')
		featurePortrayal:AddInstructions('AugmentedPath:LocalCRS,LocalCRS,LocalCRS')

		featurePortrayal:SimpleLineStyle('solid',1.28,'OUTLW')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:SimpleLineStyle('solid',0.64,sectorColourToken)
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		featurePortrayal:AddInstructions('ClearGeometry;FontColor:CHBLK')
		featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignHorizontal:Start;TextAlignVertical:Center')

		local description = LITDSN02(categoryOfLight, feature.rhythmOfLight, feature.colour, feature.height, feature['!valueOfNominalRange'], feature.status)
		if contains(feature.signalGeneration, {5,6})
		then
			description = description .. '(man)'
		end
		local textOffsetLines = featurePortrayal:GetColocatedTextCount()	-- lines of text output by co-located features
		if textOffsetLines > 0 then
			featurePortrayal:AddInstructions('TextVerticalOffset:' .. textOffsetLines * -3.51)
		end
		featurePortrayal:AddTextInstruction(description, 23, 24, 27070, 21, true)
	else
		LightFlareAndDescription(feature, featurePortrayal, contextParameters, feature.categoryOfLight[1], 27070, 21)
	end

	return 27070
end
