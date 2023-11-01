-- Main entry point for feature type.

-- UNOFFICIAL:  Rules extracted from S-52 lookup table for BRIDGE.

function SpanFixed(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR;NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceFixed.verticalClearanceValue.
		if feature.verticalClearanceFixed and feature.verticalClearanceFixed.verticalClearanceValue then
			featurePortrayal:AddInstructions('LinePlacement:Relative,0.5;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(feature.verticalClearanceFixed.verticalClearanceValue, 'clr %4.1f'), 11, 24, 12210, 24)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceFixed.verticalClearanceValue.
		if feature.verticalClearanceFixed and feature.verticalClearanceFixed.verticalClearanceValue then
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;LinePlacement:Relative,0.5;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(feature.verticalClearanceFixed.verticalClearanceValue, 'clr %4.1f'), 11, 24, 12210, 24)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 12210
end
