-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0
-- Developed for S-131 by RMM
-- TODO: portrayal for multiple CATHAF values for area features
-- TODO clarify if color fill is required for area portrayal and adapt area portrayals if needed

-- Harbour area section main entry point.
function HarbourAreaSection(feature, featurePortrayal, contextParameters)
	local viewingGroup

  if feature.PrimitiveType == PrimitiveType.Point then
  	if contextParameters.SimplifiedPoints then
      -- simplified portrayal for all categories
      viewingGroup = 32410
    	if contextParameters.RadarOverlay then
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
    	else
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
    	end
      featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
    elseif feature.categoryOfPortSection then
      -- preferred portrayal with categoryOfPortSection values
      viewingGroup = 32410
    	if contextParameters.RadarOverlay then
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
    	else
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
    	end
      if contains(1, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC96')
      elseif contains(3, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC97')
      elseif contains(8, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC92')
      elseif contains(9, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC93')
      elseif contains(11, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC94')
      elseif contains(12, feature.categoryOfPortSection) then
        featurePortrayal:AddInstructions('PointInstruction:HRBSEC95')
      else
        featurePortrayal:AddInstructions('PointInstruction:QUESMRK1')
      end
    elseif feature.categoryOfHarbourFacility then
      -- secondary portrayal with categoryOfHarbourFacility values
    	viewingGroup = 32410
    	if contextParameters.RadarOverlay then
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
    	else
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
    	end
      if contains(1, feature.categoryOfHarbourFacility) then
        -- 1: RoRo terminal, not in S-131 1.0.0 CATHAF
    	 featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
    	elseif contains(4, feature.categoryOfHarbourFacility) then
        -- 4: fishing harbour
    		featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
    	elseif contains(5, feature.categoryOfHarbourFacility) then
        -- 5: Yacht harbour/marina
    		featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
      elseif contains(6, feature.categoryOfHarbourFacility) then
        -- 6 : Naval Base
    		featurePortrayal:AddInstructions('PointInstruction:HRBFAC91')
      elseif contains(9, feature.categoryOfHarbourFacility) then
        -- 9 : Shipyard
    		featurePortrayal:AddInstructions('PointInstruction:HRBFAC93')
      elseif contains(14, feature.categoryOfHarbourFacility) then
        -- 14 : Service Harbour
    		featurePortrayal:AddInstructions('PointInstruction::HRBFAC95')
      elseif contains(15, feature.categoryOfHarbourFacility) then
        -- 15 : Pilotage Service
    		featurePortrayal:AddInstructions('PointInstruction::HRBFAC96')
      elseif contains(16, feature.categoryOfHarbourFacility) then
        -- 16 : Service and Repair
    		featurePortrayal:AddInstructions('PointInstruction:HRBFAC97')
      elseif contains(17, feature.categoryOfHarbourFacility) then
        -- 17 : Quarantine Station
    		featurePortrayal:AddInstructions('PointInstruction:HRBFAC98')
    	else
    		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
    	end
    else      -- points with neither categoryOfPortSection nor CATHAF populated
    	viewingGroup = 32410
    	if contextParameters.RadarOverlay then
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
    	else
    		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
    	end
    	featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
     end
     -- end of portrayal for point features
  elseif feature.PrimitiveType == PrimitiveType.Surface then
  	if contextParameters.PlainBoundaries then
  		viewingGroup = 32410
  		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:6;DisplayPlane:UnderRADAR')
  		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
  		featurePortrayal:AddInstructions('LineInstruction:_simple_')
  	else
  		viewingGroup = 32410
  		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:6;DisplayPlane:UnderRADAR')
      if feature.categoryOfPortSection then
        if contains(1, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('PointInstruction:HRBSEC96C')
        elseif contains(3, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('PointInstruction:HRBSEC97C')
        elseif contains(8, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('PointInstruction:HRBSEC92C')
        elseif contains(9, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('ColorFill:DEPVS')
		      featurePortrayal:AddInstructions('AreaFillReference:DRGARE01')
		      -- featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		      -- featurePortrayal:AddInstructions('LineInstruction:_simple_')
        elseif contains(11, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('ColorFill:DEPVS')
          featurePortrayal:AddInstructions('PointInstruction:HRBSEC94C')
        elseif contains(12, feature.categoryOfPortSection) then
          featurePortrayal:AddInstructions('PointInstruction:HRBSEC95C')
        elseif feature.categoryOfHarbourFacility then
          featurePortrayal:AddInstructions('PointInstruction:MULTIC91C')
        else
          featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
        end
      else
        featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
      end
      -- if simple lines parameter not set, then use same line style for all area features
  		featurePortrayal:AddInstructions('LineInstruction:HRBSEC01')
    end
    -- end of portrayal for surface features
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
