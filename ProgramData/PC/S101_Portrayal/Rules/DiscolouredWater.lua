-- DiscolouredWater portrayal rules file.
--
-- ISSUES: PSWG #61, PC #40, PC #126
--
-- Main entry point for feature type.
function DiscolouredWater(feature, featurePortrayal, contextParameters)

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53017,53017')
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:EMDSCWTR')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		
		featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:DSCWTR51')
		
		if contextParameters.PlainBoundaries then
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else 
			featurePortrayal:AddInstructions('ViewingGroup:26150;DrawingPriority:9;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('LineInstruction:DSCWTR51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 26150
end
