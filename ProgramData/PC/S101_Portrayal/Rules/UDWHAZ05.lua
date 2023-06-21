-- UDWHAZ05 conditional symbology rules file.

-- Main entry point for CSP.
function UDWHAZ05(feature, featurePortrayal, contextParameters, DEPTH_VALUE)
	Debug.StartPerformance('Lua Code - UDWHAZ05')

	if DEPTH_VALUE <= contextParameters.SafetyContour then
		if not feature.surroundingDepth or feature.surroundingDepth > contextParameters.SafetyContour then
			-- DANGER = true
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				featurePortrayal:AddInstructions('ViewingGroup:14050;DrawingPriority:24')
			else
				if contextParameters.RadarOverlay then
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:24;DisplayPlane:OverRADAR;ScaleMinimum:'..scaminInfinite)
				else
					featurePortrayal:AddInstructions('ViewingGroup:14010;DrawingPriority:24;DisplayPlane:UnderRADAR;ScaleMinimum:'..scaminInfinite)
				end
				return 'ISODGR01'
			end
		elseif contextParameters.ShowIsolatedDangersInShallowWaters and feature.surroundingDepth >= scaledDecimalZero and feature.surroundingDepth <= contextParameters.SafetyContour then
			if feature.waterLevelEffect == 1 or feature.waterLevelEffect == 2 then
				featurePortrayal:AddInstructions('ViewingGroup:24050;DrawingPriority:24')
			else
				if contextParameters.RadarOverlay then
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:24;DisplayPlane:OverRADAR')
				else
					featurePortrayal:AddInstructions('ViewingGroup:24020;DrawingPriority:24;DisplayPlane:UnderRADAR')
				end
				return 'ISODGR01'
			end
		end
	end

	Debug.StopPerformance('Lua Code - UDWHAZ05')
end
