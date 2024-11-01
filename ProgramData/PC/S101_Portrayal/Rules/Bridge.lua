--
-- #306, #312
--
-- Bridge main entry point.
function Bridge(feature, featurePortrayal, contextParameters)
	local viewingGroup = 12210

	if feature.PrimitiveType ~= PrimitiveType.None then
		featurePortrayal:AddInstructions('AlertReference:NavHazard')
	end

	if contextParameters.RadarOverlay then
		featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRadar')
	else
		featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRadar')
	end

	-- if over navigable water: "opening bridge" is mandatory
	-- If "opening bridge" = True: category of opening bridge
	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.openingBridge == true then
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		else
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.openingBridge == true then
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		else
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.openingBridge == true then
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		else
			featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end		
	elseif feature.PrimitiveType == PrimitiveType.None then
		viewingGroup = 12210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRadar;NullInstruction')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRadar;NullInstruction')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	if feature.PrimitiveType ~= PrimitiveType.None and feature.featureName[1] and feature.featureName[1].name then
		-- Note: S-52 only shows OBJNAM when CATBRG is not 2-8. It uses an offset of 3.51,0.
		-- In S-101 the label is shown when available, and is offset further from the object.
		-- May need additional refinement to prevent the label from overwriting the clearance value(s).
		featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontColor:CHBLK')
		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), '%s'), 21, 24, 12210, 24)
	end

	return viewingGroup
end
