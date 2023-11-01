-- FoulGround portrayal rules file.
-- UNOFFICIAL:  Rules extracted from S-52 lookup table for OBSTRN where CATOBS = 7.
--
-- ISSUES: PC #72, #39, PSWG #65
-- #166

-- Main entry point for feature type.
function FoulGround(feature, featurePortrayal, contextParameters)
	local viewingGroup = 34050
	local VOS = feature.valueOfSounding

	if VOS then
			viewingGroup = 34051
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
	else
		featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
	end

	if feature.PrimitiveType == PrimitiveType.Point then
		featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
	-- #166: curve geometry removed in FC 1.1.0
	--elseif feature.PrimitiveType == PrimitiveType.Curve then
		--featurePortrayal:AddInstructions('LineInstruction:FOULGRD1')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('PointInstruction:FOULGD51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
