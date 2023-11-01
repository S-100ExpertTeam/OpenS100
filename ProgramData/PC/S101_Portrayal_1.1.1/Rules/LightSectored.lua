-- LightSectored portrayal rules file.
-- #96
-- #100
-- PC #113, PSWG #52
-- #20
-- #155
-- #10
-- #171
-- #174
-- #188
-- #190

local function nmi2metres(nmi)
	return nmi * 1852.0
end

-- Sector values are given from seaward, this function flips them.
local function flipSector(sector)
	if sector < 180.0 then
		return sector + 180.0
	else
		return sector - 180.0
	end
end

-- Main entry point for feature type.
function LightSectored(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType ~= PrimitiveType.Point then
		error('LightSectored must be of spatial type point')
	end

	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:OverRADAR;Hover:true')
	else
		featurePortrayal:AddInstructions('ViewingGroup:27070;DrawingPriority:24;DisplayPlane:UnderRADAR;Hover:true')
	end

	local informationFound = false

	for isc, sectorCharacteristic in ipairs(feature.sectorCharacteristics) do
		for ils, lightSector in ipairs(sectorCharacteristic.lightSector) do
			informationFound = informationFound or (lightSector.sectorInformation and #lightSector.sectorInformation ~= 0)

			local valueOfNominalRange = 9.0

			if lightSector.valueOfNominalRange then
				valueOfNominalRange = lightSector.valueOfNominalRange:ToNumber()
			end

			local colour = lightSector.colour
			local sectorColourToken
			local sectorLightSymbol

			if colour[1] == 1 and colour[2] == 3 then -- white and red
				sectorColourToken = 'LITRD'
				sectorLightSymbol = 'LIGHTS11'
			elseif colour[1] == 3 then -- red
				sectorColourToken = 'LITRD'
				sectorLightSymbol = 'LIGHTS11'
			elseif colour[1] == 1 and colour[2] == 4 then -- white and green
				sectorColourToken = 'LITGN'
				sectorLightSymbol = 'LIGHTS12'
			elseif colour[1] == 4 then -- green
				sectorColourToken = 'LITGN'
				sectorLightSymbol = 'LIGHTS12'
			elseif colour[1] == 11 or colour[1] == 6 or colour[1] == 1 then -- orange or yellow or white
				sectorColourToken = 'LITYW'
				sectorLightSymbol = 'LIGHTS13'
			elseif colour[1] == 1 and colour[2] == 6 then -- blue and yellow
				sectorColourToken = 'LITYW'
				sectorLightSymbol = 'LIGHTS13'
			else
				sectorColourToken = 'CHMGD'
				sectorLightSymbol = 'LITDEF11'
			end

			if lightSector.sectorLimit then
				-- Sectored light.
				local sectorLimit1 = flipSector(lightSector.sectorLimit.sectorLimitOne.sectorBearing:ToNumber())
				local sectorLimit2 = flipSector(lightSector.sectorLimit.sectorLimitTwo.sectorBearing:ToNumber())

				if sectorLimit2 < sectorLimit1 then
					sectorLimit2 = sectorLimit2 + 360.0
				end

				local length1, length2
				local crs1, crs2

				if contextParameters.FullLightLines then
					length1 = nmi2metres(valueOfNominalRange)
					crs1 = 'GeographicCRS'
					
					length2 = length1
					crs2 = crs1
				else
					if lightSector.sectorLimit.sectorLimitOne.sectorLineLength then
						length1 = nmi2metres(lightSector.sectorLimit.sectorLimitOne.sectorLineLength:ToNumber())
						crs1 = 'GeographicCRS'
					else
						length1 = 25.0
						crs1 = 'LocalCRS'
					end
					
					if lightSector.sectorLimit.sectorLimitTwo.sectorLineLength then
						length2 = nmi2metres(lightSector.sectorLimit.sectorLimitTwo.sectorLineLength:ToNumber())
						crs2 = 'GeographicCRS'
					else
						length2 = length1
						crs2 = crs1
					end
				end

				-- Draw leg 1
				featurePortrayal:AddInstructions('AugmentedRay:GeographicCRS,' .. sectorLimit1 .. ',' .. crs1 .. ',' .. length1)
				featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
				
				-- Draw leg 2
				featurePortrayal:AddInstructions('AugmentedRay:GeographicCRS,' .. sectorLimit2 .. ',' .. crs2 .. ',' .. length2)
				featurePortrayal:AddInstructions('LineInstruction:_simple_')

				-- Draw sector arcs
				if lightSector.sectorExtension then
					featurePortrayal:AddInstructions('ArcByRadius:0,0,25,' .. sectorLimit1 .. ',' .. sectorLimit2 - sectorLimit1)
				else
					featurePortrayal:AddInstructions('ArcByRadius:0,0,20,' .. sectorLimit1 .. ',' .. sectorLimit2 - sectorLimit1)
				end
				featurePortrayal:AddInstructions('AugmentedPath:LocalCRS,GeographicCRS,LocalCRS')

				if contains(lightSector.lightVisibility, { 7, 6, 3 }) then
					featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK') -- Redundant?
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				else
					featurePortrayal:SimpleLineStyle('solid',1.28,'OUTLW')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
					featurePortrayal:SimpleLineStyle('solid',0.64,sectorColourToken)
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				end
			elseif lightSector.directionalCharacter then
				-- Directional light.
				local leglen = nmi2metres(valueOfNominalRange)
				local orientationValue = lightSector.directionalCharacter.orientation.orientationValue
				local orientation
				
				if orientationValue
				then
					-- Draw the direction marked by the light (bearing is from seaward)
					orientation = flipSector(orientationValue:ToNumber())

					featurePortrayal:AddInstructions('AugmentedRay:GeographicCRS,' .. orientation .. ',GeographicCRS,' .. leglen)
					featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				end

				local categoryOfLight = feature.categoryOfLight

				if valueOfNominalRange >= 10.0 and not contains(feature.categoryOfLight, { 5 }) and sectorCharacteristic.lightCharacteristic ~= 12 then
					featurePortrayal:AddInstructions('ArcByRadius:0,0,25,0,360')
					featurePortrayal:AddInstructions('AugmentedPath:LocalCRS,LocalCRS,LocalCRS')

					featurePortrayal:SimpleLineStyle('solid',1.28,'OUTLW')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
					featurePortrayal:SimpleLineStyle('solid',0.64,sectorColourToken)
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				elseif orientationValue then
					featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. orientation)
					featurePortrayal:AddInstructions('ClearGeometry;PointInstruction:' .. sectorLightSymbol)
					featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
				else
					featurePortrayal:AddInstructions('ClearGeometry;PointInstruction:QUESMRK1')
				end

				featurePortrayal:AddInstructions('LocalOffset:10.53,-3.51;FontSize:10;FontColor:CHBLK;ClearGeometry')
				
				if orientationValue
				then
					featurePortrayal:AddInstructions('TextAlignVertical:Top')
					featurePortrayal:AddTextInstruction(string.format('%03.0f deg', orientationValue:ToNumber()), 23, 24, 27070, 24)
				end

				local description = LITDSN02(feature.categoryOfLight[1], sectorCharacteristic, colour, feature.height, lightSector.valueOfNominalRange, feature.status)

				featurePortrayal:AddInstructions('TextAlignVertical:Bottom')
				featurePortrayal:AddTextInstruction(EncodeString(description), 23, 24, 27070, 24)

				-- PC #113
				if lightSector.directionalCharacter.moireEffect then
					if orientationValue then
						featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. orientation)
						featurePortrayal:AddInstructions('ClearGeometry;LocalOffset:0,0;PointInstruction:MOIRE01')
						featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
					else
						featurePortrayal:AddInstructions('ClearGeometry;LocalOffset:0,0;PointInstruction:MOIRE01')
					end
				end
			else
				-- Neither sectorLimit nor directionalCharacter was found.
				featurePortrayal:AddInstructions('ClearGeometry;PointInstruction:QUESMRK1')
			end
		end
	end

	if informationFound then
		featurePortrayal:AddInstructions('LocalOffset:0,0;LinePlacement:Relative,0.5;AreaPlacement:VisibleParts;AreaCRS:GlobalGeometry;Rotation:PortrayalCRS,0;ScaleFactor:1;ClearGeometry')

		featurePortrayal:AddInstructions('Hover:true')

		featurePortrayal:AddInstructions('ViewingGroup:27070,31030;DrawingPriority:24;PointInstruction:INFORM01')
	end

	return 27070
end
