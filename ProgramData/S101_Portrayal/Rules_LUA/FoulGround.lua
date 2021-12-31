-- FoulGround portrayal rules file.

-- UNOFFICIAL:  Rules extracted from S-52 lookup table for OBSTRN where CATOBS = 7.

-- Main entry point for feature type.
function FoulGround(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		if feature.valueOfSounding then
			featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		error('Not Implemented: No curve symbology defined in S-52 for FoulGround')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.valueOfSounding then
			featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.valueOfSounding then
			featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('PointInstruction:testPCB')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
