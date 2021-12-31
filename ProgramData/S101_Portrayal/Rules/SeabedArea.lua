-- Main entry point for feature type.
function SeabedArea(feature, featurePortrayal, contextParameters)
	local function AddNatureOfSurfaceText(priority)
		local natureOfSurfaceAbbrev =
		{
			[1] = 'M',
			[2] = 'Cy',
			[3] = 'Si',
			[4] = 'S',
			[5] = 'St',
			[6] = 'G',
			[7] = 'P',
			[8] = 'Cb',
			[9] = 'R',
			[11] = 'R',
			[14] = 'Co',
			[17] = 'Sh',
			[18] = 'R'
		}

		if feature.surfaceCharacteristics then
			local text

			for _, surfaceCharacteristic in ipairs(feature.surfaceCharacteristics) do
				if surfaceCharacteristic.natureOfSurface then
					if text then
						text = text .. ' ' .. natureOfSurfaceAbbrev[surfaceCharacteristic.natureOfSurface]
					else
						text = natureOfSurfaceAbbrev[surfaceCharacteristic.natureOfSurface]
					end
				end
			end

			if text then
				featurePortrayal:AddInstructions('FontSize:10;FontColor:CHBLK;TextAlignHorizontal:Center;TextAlignVertical:Center')
				featurePortrayal:AddTextInstruction(text, 25, 24, 34010, priority)
			end
		end
	end

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		AddNatureOfSurfaceText(12)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		AddNatureOfSurfaceText(12)
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.waterLevelEffect == 3 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			AddNatureOfSurfaceText(9)
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 9 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 11 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 14 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			AddNatureOfSurfaceText(9)
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			end
			AddNatureOfSurfaceText(9)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.waterLevelEffect == 3 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			AddNatureOfSurfaceText(9)
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 9 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 11 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface == 14 then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:RCKLDG01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.waterLevelEffect == 4 and feature.surfaceCharacteristics[1] and feature.surfaceCharacteristics[1].natureOfSurface then
			featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			AddNatureOfSurfaceText(9)
		else
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			end
			AddNatureOfSurfaceText(9)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	if #featurePortrayal.DrawingInstructions == 0 then
		featurePortrayal:AddInstructions('ViewingGroup:34010;DrawingPriority:9;DisplayPlane:UnderRADAR;NullInstruction')
	end

	return 34010
end
