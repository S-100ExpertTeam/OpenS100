-- LightAllAround portrayal rules file.

-- Referenced CSPs.
require 'LightFlareAndDescription'
require 'LITDSN02'

-- Main entry point for feature type.
function LightAllAround(feature, featurePortrayal, contextParameters)
	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:21;DisplayPlane:OverRADAR;Hover:true')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:21;DisplayPlane:UnderRADAR;Hover:true')
	end

	local valueOfNominalRange = 9.0

	if feature.valueOfNominalRange then
		valueOfNominalRange = feature.valueOfNominalRange:ToNumber()
	end

	local categoryOfLight = feature.categoryOfLight or {}

	if contains(categoryOfLight, { 8, 11 }) then
		-- Flood or spot light
		featurePortrayal:AddInstructions('PointInstruction:LIGHTS82')
	elseif contains(categoryOfLight, { 9 }) then
		-- Strip light
		featurePortrayal:AddInstructions('PointInstruction:LIGHTS81')
	elseif valueOfNominalRange >= 10.0 and not contains(categoryOfLight, { 5 }) and feature.rhythmOfLight.lightCharacteristic ~= 12 then
		-- Non aero and non morse code light visible at ten miles.
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

		featurePortrayal:AddInstructions('ClearGeometry;Rotation:PortrayalCRS,0;FontSize:10;FontColor:CHBLK')
		featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignHorizontal:Start;TextAlignVertical:Center')

		local description = LITDSN02(categoryOfLight, feature.rhythmOfLight, feature.colour, feature.height, feature['!valueOfNominalRange'], feature.status)
		featurePortrayal:AddTextInstruction(description, 23, 24, 27070, 21)
	else
		LightFlareAndDescription(feature, featurePortrayal, contextParameters, feature.categoryOfLight[1], 27070, 21)
	end

	return 27070
end
