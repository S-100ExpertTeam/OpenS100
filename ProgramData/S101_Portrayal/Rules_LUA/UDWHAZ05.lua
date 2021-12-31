-- UDWHAZ05 conditional symbology rules file.

-- Main entry point for CSP.
function UDWHAZ05(feature, featurePortrayal, contextParameters, DEPTH_VALUE)
	Debug.StartPerformance('Lua Code - UDWHAZ05')

	if DEPTH_VALUE <= contextParameters.SAFETY_CONTOUR then
		if not feature.surroundingDepth or feature.surroundingDepth > contextParameters.SAFETY_CONTOUR then
			-- DANGER = true
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				featurePortrayal:AddInstructions('ViewingGroup:14050;DrawingPriority:8')
			else
				if contextParameters.RADAR_OVERLAY then
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:8;DisplayPlane:OverRADAR;ScaleMinimum:'..scaminInfinite)
				else
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:8;DisplayPlane:UnderRADAR;ScaleMinimum:'..scaminInfinite)
				end
				return 'ISODGR01'
			end
		elseif contextParameters.SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS and feature.surroundingDepth >= scaledDecimalZero and feature.surroundingDepth <= contextParameters.SAFETY_CONTOUR then
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				featurePortrayal:AddInstructions('ViewingGroup:24050;DrawingPriority:8')
			else
				if contextParameters.RADAR_OVERLAY then
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:8;DisplayPlane:OverRADAR')
				else
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:8;DisplayPlane:UnderRADAR')
				end
				return 'ISODGR01'
			end
		end
	end

	Debug.StopPerformance('Lua Code - UDWHAZ05')
end
