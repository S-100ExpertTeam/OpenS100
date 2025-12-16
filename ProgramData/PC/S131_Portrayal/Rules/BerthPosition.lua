-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2022/11/28)
-- New rule for S-131 1.0.0 by RMM

-- BerthPosition main entry point.
function BerthPosition(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 32440
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
    -- TODO - symbols corresponding to ramp, metre mark, manifold nubers
    if (feature.bollardNumber) then
      featurePortrayal:AddInstructions('PointInstruction:PILPNT02')
      featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
    else
  		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
		  featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
    end
    -- add code to get bollard numbers, metre mark numbers, manifold numbers, ramp number and add a text instruction
    -- note the preference order in case more than one of the above are populated
    -- TODO - handle ranges of numbers, 1 for ramp number, 1 or 2 for the others

    if (feature.bollardNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
  		featurePortrayal:AddTextInstruction(EncodeString(feature.bollardNumber[1], 'Bol %s'), 29, 24, 32440, 15)
    elseif (feature.metreMarkNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
  		featurePortrayal:AddTextInstruction(EncodeString(feature.metreMarkNumber[1], 'MM %s'), 29, 24, 32440, 15)
    elseif (feature.manifoldNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
  		featurePortrayal:AddTextInstruction(EncodeString(feature.manifoldNumber[1], 'MnF %s'), 29, 24, 32440, 15)
    elseif (feature.rampNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
  		featurePortrayal:AddTextInstruction(EncodeString(feature.rampNumber[1], 'Rp %s'), 29, 24, 32440, 15)
    else
      Debug.Trace('Warning: BerthPosition without an identifying number')
    end

--		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 15)
--	elseif feature.PrimitiveType == PrimitiveType.Curve then
--		viewingGroup = 32440
--		if contextParameters.RadarOverlay then
--			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRADAR')
--		else
--			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRADAR')
--		end
--		featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRD')
--		featurePortrayal:AddInstructions('LineInstruction:_simple_')
--		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
--		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
--		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 15)
--	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
--		viewingGroup = 32440
--		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:9;DisplayPlane:UnderRADAR')
--		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
--		featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
--		featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 9)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
