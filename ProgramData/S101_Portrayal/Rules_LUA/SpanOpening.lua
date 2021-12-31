-- Main entry point for feature type.

-- UNOFFICIAL:  Rules extracted from S-52 lookup table for BRIDGE.

function SpanOpening(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:')
		featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceClosed.verticalClearanceValue.
		if feature.verticalClearanceClosed and feature.verticalClearanceClosed.verticalClearanceValue then
			featurePortrayal:AddInstructions('ViewingGroup:11')
			featurePortrayal:AddInstructions('LinePlacement:Relative,0.5;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextInstruction:' .. EncodeString(feature.verticalClearanceClosed.verticalClearanceValue, 'clr cl %4.1f') .. ',11,8')
		end

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceOpen.verticalClearanceValue.
		if feature.verticalClearanceOpen and feature.verticalClearanceOpen.verticalClearanceValue then
			featurePortrayal:AddInstructions('ViewingGroup:11')
			featurePortrayal:AddInstructions('LinePlacement:Relative,0.5;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;TextInstruction:' .. EncodeString(feature.verticalClearanceOpen.verticalClearanceValue, 'clr op %4.1f') .. ',11,8')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:')

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceClosed.verticalClearanceValue.
		if feature.verticalClearanceClosed and feature.verticalClearanceClosed.verticalClearanceValue then
			featurePortrayal:AddInstructions('ViewingGroup:11')
			featurePortrayal:AddInstructions('FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextInstruction:' .. EncodeString(feature.verticalClearanceClosed.verticalClearanceValue, 'clr cl %4.1f') .. ',11,8')
		end

		-- This if statement is here since the ESRI converter doesn't always emit the mandatory attribute verticalClearanceOpen.verticalClearanceValue.
		if feature.verticalClearanceOpen and feature.verticalClearanceOpen.verticalClearanceValue then
			featurePortrayal:AddInstructions('ViewingGroup:11')
			featurePortrayal:AddInstructions('FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;TextInstruction:' .. EncodeString(feature.verticalClearanceOpen.verticalClearanceValue, 'clr op %4.1f') .. ',11,8')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
