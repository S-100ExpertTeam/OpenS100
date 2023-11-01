-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- PC Issue #73, PSWG #102

-- Local magnetic anomaly main entry point.
function LocalMagneticAnomaly(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 31080
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG01')
		

	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 31080
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG01')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 31080
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 31080
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG51')
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	-- issue #73 PC, #102 PSWG
	-- DEBUG TESTING SUPPORT

	-- replace with local valueOfLocalMagneticAnomaly = feature.valueOfLocalMagneticAnomaly 
	-- valueOf multiplicity is 1:2
	--local valueOfLocalMagneticAnomaly = {'1', nil} -- replace with local valueOfLocalMagneticAnomaly = feature.valueOfLocalMagneticAnomaly  
		
	--local valueOfLocalMagneticAnomaly = {}
	
	--Debug.Break()
	--valueOfLocalMagneticAnomaly[1] = {magneticAnomalyValue = 175, referenceDirection = 5} --RefDir 5 = East, 13 = West 
	--valueOfLocalMagneticAnomaly[2] = {magneticAnomalyValue = 145, referenceDirection = unknownValue} --RefDir 5 = East, 13 = West 
	-- END 	-- DEBUG TESTING SUPPORT

	local valueOfLocalMagneticAnomaly = feature.valueOfLocalMagneticAnomaly   -- DEBUG as {}

	local magCount = #valueOfLocalMagneticAnomaly
	
	featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10;FontColor:CHMGF')

	if magCount == 1 then
		if valueOfLocalMagneticAnomaly[1].magneticAnomalyValue ~= nil then
			if valueOfLocalMagneticAnomaly[1].magneticAnomalyValue ~= unknownValue then
				if valueOfLocalMagneticAnomaly[1].referenceDirection ~= nil then
					if valueOfLocalMagneticAnomaly[1].referenceDirection == 5 then
						featurePortrayal:AddTextInstruction(EncodeString(valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, '(%.0f°E)'), 30, 24, viewingGroup, 12)
					elseif  valueOfLocalMagneticAnomaly[1].referenceDirection == 13 then -- West
						featurePortrayal:AddTextInstruction(EncodeString(valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, '(%.0f°W)'), 30, 24, viewingGroup, 12)
					elseif valueOfLocalMagneticAnomaly[1].referenceDirection == unknownValue then
						featurePortrayal:AddTextInstruction(EncodeString(valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, '(±%.0f°)'), 30, 24, viewingGroup, 12)
					end
				else
					featurePortrayal:AddTextInstruction(EncodeString(valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, '(±%.0f°)'), 30, 24, viewingGroup, 12)
				end
			else
				-- depict text "Local Magnetic Anomaly" - Colour (CHMGF to match LOCMAG51)
				featurePortrayal:AddInstructions('TextAlignVertical:Center;FontWeight:Light;FontColor:CHMGF')
				featurePortrayal:AddTextInstruction("Local Magnetic Anomaly", 30, 24, viewingGroup, 12)
			end
		else
			error('Invalid primitive type or mariner settings passed to portrayal')
		end
	elseif magCount == 2 then
		if valueOfLocalMagneticAnomaly[1].magneticAnomalyValue ~= nil and valueOfLocalMagneticAnomaly[2].magneticAnomalyValue ~= nil then

			local txtStr
			local dir1 = ''
			local dir2 = ''
			if valueOfLocalMagneticAnomaly[1].magneticAnomalyValue ~= unknownValue and valueOfLocalMagneticAnomaly[2].magneticAnomalyValue ~= unknownValue then
				if (valueOfLocalMagneticAnomaly[1].referenceDirection ~= unknownValue and valueOfLocalMagneticAnomaly[2].referenceDirection ~= unknownValue) and
					(valueOfLocalMagneticAnomaly[1].referenceDirection ~= nil and valueOfLocalMagneticAnomaly[2].referenceDirection ~= nil) then
					if valueOfLocalMagneticAnomaly[1].referenceDirection == 5 then dir1 = 'E' end
					if valueOfLocalMagneticAnomaly[2].referenceDirection == 5 then dir2 = 'E' end
					if valueOfLocalMagneticAnomaly[1].referenceDirection == 13 then dir1 = 'W' end
					if valueOfLocalMagneticAnomaly[2].referenceDirection == 13 then dir2 = 'W' end

						txtStr = string.format('(%.0f°%s/%.0f°%s)',valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, dir1, valueOfLocalMagneticAnomaly[2].magneticAnomalyValue, dir2)
						featurePortrayal:AddTextInstruction(txtStr, 30, 24, viewingGroup, 12)
				else
					txtStr = string.format('(%.0f°%s/%.0f°%s)',valueOfLocalMagneticAnomaly[1].magneticAnomalyValue, dir1, valueOfLocalMagneticAnomaly[2].magneticAnomalyValue, dir2)
					featurePortrayal:AddTextInstruction(txtStr, 30, 24, viewingGroup, 12)
				end
			elseif valueOfLocalMagneticAnomaly[1].magneticAnomalyValue == unknownValue or valueOfLocalMagneticAnomaly[2].magneticAnomalyValue == unknownValue then
				featurePortrayal:AddInstructions('TextAlignVertical:Center;FontWeight:Light;FontColor:CHMGF')
				featurePortrayal:AddTextInstruction("Local Magnetic Anomaly", 30, 24, viewingGroup, 12)
			end
		else
			error('Invalid primitive type or mariner settings passed to portrayal')
		end
	end
		
	return viewingGroup
end
