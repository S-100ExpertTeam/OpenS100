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
	
	featurePortrayal:AddInstructions('ViewingGroup:31010,accuracy;DrawingPriority:12;DisplayPlane:UnderRADAR')

	local DRVAL1 = feature.depthRangeMinimumValue
	local DRVAL2 = feature.depthRangeMaximumValue
	local intersects = (DRVAL1 == nil or DRVAL1 < contextParameters.SafetyContour) and (DRVAL2 == nil or contextParameters.SafetyContour <= DRVAL2)
	
	local dateDependent = false
	if zonesOfConfidence and #zonesOfConfidence > 0 then
		for _, zoneOfConfidence in ipairs(zonesOfConfidence) do
			dateDependent = ProcessFixedDateRange(featurePortrayal, zoneOfConfidence.fixedDateRange) or dateDependent
			if intersects then
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
					-- default without CATZOC: "M_QUAL","","AP(NODATA03);LS(DASH,2,CHGRD)","4","S","OTHER","31010" 
					featurePortrayal:AddInstructions('AreaFillReference:NODATA03')
					featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
					featurePortrayal:AddInstructions('LineInstruction:_simple_')
				end
			else
				featurePortrayal:AddInstructions('NullInstruction')
			end
			if dateDependent then
				AddDateDependentSymbol(feature, featurePortrayal, contextParameters, '31010,accuracy')
				featurePortrayal:AddInstructions('ClearTime')
			end
		end
	end

	return '31010,accuracy'
end
