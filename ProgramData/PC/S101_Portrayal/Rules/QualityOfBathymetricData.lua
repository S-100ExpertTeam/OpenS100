-- QualityOfBathymetricData portrayal rules file.
-- #80
-- #119
-- #178

require 'S100Scripting'

-- Main entry point for feature type.
function QualityOfBathymetricData(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType ~= PrimitiveType.Surface then
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	-- Debug.Break()  

	local catzoc
	local zonesOfConfidence = feature.zoneOfConfidence
	
	featurePortrayal:AddInstructions('ViewingGroup:90010;DrawingPriority:12;DisplayPlane:UnderRadar')

	-- QoBD has gaps of 0.1m between bottom of upper QoBD and top of lower QoBD. Ensure safety contour doesn't fall
	-- within a gap by restricting its precision.
	local safetyContour = math.ceil(contextParameters.SafetyContour:ToNumber() * 10) / 10
	local DRVAL1 = feature.depthRangeMinimumValue and feature.depthRangeMinimumValue:ToNumber()
	local DRVAL2 = feature.depthRangeMaximumValue and feature.depthRangeMaximumValue:ToNumber()
	
	local intersects = (DRVAL1 == nil or DRVAL1 <= safetyContour) and (DRVAL2 == nil or safetyContour <= DRVAL2)
	
	local dateDependent = false
	if zonesOfConfidence and #zonesOfConfidence > 0 then
		for _, zoneOfConfidence in ipairs(zonesOfConfidence) do
			if intersects then

				dateDependent = ProcessFixedDateRange(featurePortrayal, zoneOfConfidence.fixedDateRange) or dateDependent

				if zoneOfConfidence.categoryOfZoneOfConfidenceInData then
					if (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 1) then
						catzoc = 'A11'
					elseif (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 2) then
						catzoc = 'A21'
					elseif (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 3) then
						catzoc = 'B01'
					elseif (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 4) then
						catzoc = 'C01'
					elseif (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 5) then
						catzoc = 'D01'
					elseif (zoneOfConfidence.categoryOfZoneOfConfidenceInData == 6) then
						catzoc = 'U01'
					end
					-- CATZOC values defined
					featurePortrayal:AddInstructions('AreaFillReference:DQUAL' .. catzoc)
					featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')

				else
					-- default without CATZOC: "M_QUAL","","AP(NODATA03);LS(DASH,2,CHGRD)","4","S","OTHER","90010" 
					featurePortrayal:AddInstructions('AreaFillReference:NODATA03')
					featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				end
			else
				featurePortrayal:AddInstructions('NullInstruction')
			end
		end
		if dateDependent then
			AddDateDependentSymbol(feature, featurePortrayal, contextParameters, 90010)
			featurePortrayal:AddInstructions('ClearTime')
		end
	end

	return 90010
end
