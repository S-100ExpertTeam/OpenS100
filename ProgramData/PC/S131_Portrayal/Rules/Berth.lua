-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Modified for S-131 1.0.0 by RMM
-- TODO elaborate by adding bollard / metre mark / manifold / ramp number / minimum berth depth / availableBerthingLength?
-- TODO - check how Lua maintains attributes that have multiplicity 0..* or 1..*
-- TODO check behavior if there are successive text instructions each with local offset, the 12/31 build of the S-131 PC takes S-100 Table 9a-4 literally

-- Berth main entry point.
function Berth(feature, featurePortrayal, contextParameters)
	local viewingGroup
  local nameOffset = 0      -- offset for name if bollard number, etc. are displayed
  local yOffsetValue = 2.0  -- default offset in mm
  local label

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32440
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
		if (feature.bollardNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.bollardNumber > 1 then
        featurePortrayal:AddTextInstruction(EncodeString(feature.bollardNumber[1] .. '-' .. feature.bollardNumber[2], 'Bol %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.bollardNumber[1], 'Bol %s'), 29, 24, 32440, 15)
      end
		elseif (feature.metreMarkNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.metreMarkNumber > 1 then
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.metreMarkNumber[1] .. '-' .. feature.metreMarkNumber[2], 'MM %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.metreMarkNumber[1], 'MM %s'), 29, 24, 32440, 15)
      end
		elseif (feature.manifoldNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.manifoldNumber > 1 then
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.manifoldNumber[1] .. '-' .. feature.manifoldNumber[2], 'MnF %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.manifoldNumber[1], 'Mnf %s'), 29, 24, 32440, 15)
      end
		elseif (feature.rampNumber) then
      -- ramp number has only 0..1 multiplicity in S-131 1.0.0
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
  		featurePortrayal:AddTextInstruction(EncodeString(feature.rampNumber[1], 'Rp %s'), 29, 24, 32440, 15)
    end
		if feature.featureName then
		  featurePortrayal:AddInstructions('LocalOffset:3.51,' .. nameOffset .. ';FontSize:10')   -- include offset value for name 
		  featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 15)
    end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32440
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('solid',0.96,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
    label = 'Berth ' .. GetFeatureName(feature, contextParameters), 'Nr %s'
		if (feature.bollardNumber) then
      if #feature.bollardNumber > 1 then
        label = label .. ' Bol ' .. feature.bollardNumber[1] .. '-' .. feature.bollardNumber[2]
      else
  		  label = label .. ' Bol ' .. feature.bollardNumber[1]
      end
		elseif (feature.metreMarkNumber) then
      if #feature.metreMarkNumber > 1 then
  		  label = label .. ' MM ' .. feature.metreMarkNumber[1] .. '-' .. feature.metreMarkNumber[2]
      else
  		  label = label .. ' MM ' .. feature.metreMarkNumber[1]
      end
		elseif (feature.manifoldNumber) then
      if #feature.manifoldNumber > 1 then
  		  label = label .. ' Mnf ' .. feature.manifoldNumber[1] .. '-' .. feature.manifoldNumber[2]
      else
  		  label = label .. ' Mnf ' .. feature.manifoldNumber[1]
      end
		elseif (feature.rampNumber) then
  		label = label .. ' Rp ' .. feature.rampNumber[1]
    end
		featurePortrayal:AddTextInstruction(EncodeString(label), 29, 24, 32440, 15)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32440
		featurePortrayal:AddInstructions('ViewingGroup:32440;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:BRTHNO01')
		if (feature.bollardNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.bollardNumber > 1 then
        featurePortrayal:AddTextInstruction(EncodeString(feature.bollardNumber[1] .. '-' .. feature.bollardNumber[2], 'Bol %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.bollardNumber[1], 'Bol %s'), 29, 24, 32440, 15)
      end
		elseif (feature.metreMarkNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.metreMarkNumber > 1 then
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.metreMarkNumber[1] .. '-' .. feature.metreMarkNumber[2], 'MM %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.metreMarkNumber[1], 'MM %s'), 29, 24, 32440, 15)
      end
		elseif (feature.manifoldNumber) then
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
      if #feature.manifoldNumber > 1 then
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.manifoldNumber[1] .. '-' .. feature.manifoldNumber[2], 'MnF %s'), 29, 24, 32440, 15)
      else
  		  featurePortrayal:AddTextInstruction(EncodeString(feature.manifoldNumber[1], 'Mnf %s'), 29, 24, 32440, 15)
      end
		elseif (feature.rampNumber) then
      -- ramp number has only 0..1 multiplicity in S-131 1.0.0
  		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
      nameOffset = yOffsetValue
  		featurePortrayal:AddTextInstruction(EncodeString(feature.rampNumber[1], 'Rp %s'), 29, 24, 32440, 15)
    end
    if feature.featureName then
		  featurePortrayal:AddInstructions('LocalOffset:0,' .. nameOffset .. ';FontSize:10')
		  featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Nr %s'), 29, 24, 32440, 9)
    end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
