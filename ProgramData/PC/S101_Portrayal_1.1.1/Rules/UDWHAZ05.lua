-- UDWHAZ05 conditional symbology rules file.
-- #57
-- #89
-- #155

-- Main entry point for CSP.
function UDWHAZ05(feature, featurePortrayal, contextParameters, DEPTH_VALUE, originalViewingGroup)
	Debug.StartPerformance('Lua Code - UDWHAZ05')

	local viewingGroup = originalViewingGroup
	if DEPTH_VALUE <= contextParameters.SafetyContour then
		featurePortrayal:AddInstructions('AlertReference:NavHazard')
		if not feature.surroundingDepth or feature.surroundingDepth >= contextParameters.SafetyContour then
			-- DANGER = true
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				viewingGroup = 14050
				featurePortrayal:AddInstructions('ViewingGroup:14050;DrawingPriority:24')
			else
				viewingGroup = 14010
				if contextParameters.RadarOverlay then
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:24;DisplayPlane:OverRADAR;ScaleMinimum:'..scaminInfinite)
				else
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:24;DisplayPlane:UnderRADAR;ScaleMinimum:'..scaminInfinite)
				end
				Debug.StopPerformance('Lua Code - UDWHAZ05')
				return 'ISODGR01', viewingGroup
			end
		elseif contextParameters.ShallowWaterDangers and feature.surroundingDepth >= scaledDecimalZero and feature.surroundingDepth < contextParameters.SafetyContour then
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				viewingGroup = 24050
				featurePortrayal:AddInstructions('ViewingGroup:24050;DrawingPriority:24')
			else
				viewingGroup = 24020
				if contextParameters.RadarOverlay then
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:24;DisplayPlane:OverRADAR')
				else
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:24;DisplayPlane:UnderRADAR')
				end
				Debug.StopPerformance('Lua Code - UDWHAZ05')
				return 'ISODGR01', viewingGroup
			end
		end
	end

	Debug.StopPerformance('Lua Code - UDWHAZ05')
	return nil, viewingGroup
end
