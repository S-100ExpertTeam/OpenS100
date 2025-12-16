-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2022/12/31)
-- Developed for S131 by RMM

-- Terminal main entry point.
function Terminal(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then   -- same portrayal for simplified and complex points
    -- same viewing group, drawing priority, and display plane for all choices
		viewingGroup = 32410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
    if #feature.product == 1 and (contains(1, feature.product) or contains(7, feature.product) or contains(18, feature.product) or contains(19, feature.product)) then
      -- if only one specialized product prefer portrayal according to product
      if contains(1, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMOIL91')
      elseif contains(7, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMCHE91')
      elseif contains(18, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMLNG91')
      elseif contains(19, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMLPG91')
      else    -- catchall, should not execute unless condition in containing IF changes
        featurePortrayal:AddInstructions('PointInstruction:TRMGEN91')
      end
		elseif contains(1, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif contains(3, feature.categoryOfHarbourFacility) then
      featurePortrayal:AddInstructions('PointInstruction:TERMNL92')
		--[[
		elseif contains(4, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		]]--
		elseif contains(5, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		elseif contains(7, feature.categoryOfHarbourFacility) then -- 7 tanker terminal
			featurePortrayal:AddInstructions('PointInstruction:TRMTNK91')
		elseif contains(8, feature.categoryOfHarbourFacility) then -- 8 passenger terminal
      featurePortrayal:AddInstructions('PointInstruction:TERMNL93')
		elseif contains(10, feature.categoryOfHarbourFacility) then -- 10 Container terminal
      featurePortrayal:AddInstructions('PointInstruction:TERCON91')
		elseif contains(11, feature.categoryOfHarbourFacility) then -- 11 bulk terminal
      featurePortrayal:AddInstructions('PointInstruction:TERMNL94')
		else
			featurePortrayal:AddInstructions('PointInstruction:TRMGEN91')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then  -- same portrayal for simple and complex boundaries
    -- same viewing group and display plane for all choices
		viewingGroup = 32410
		featurePortrayal:AddInstructions('ViewingGroup:25030;DrawingPriority:12;DisplayPlane:UnderRADAR')
    if #feature.product == 1 and (contains(1, feature.product) or contains(7, feature.product) or contains(18, feature.product) or contains(19, feature.product)) then
      -- if only one specialized product prefer portrayal according to product
      if contains(1, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMOIL91')
  			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
        featurePortrayal:AddInstructions('LineInstruction:_simple_')
      elseif contains(7, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMCHE91')
  			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
        featurePortrayal:AddInstructions('LineInstruction:_simple_')
      elseif contains(18, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMLNG91')
  			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
        featurePortrayal:AddInstructions('LineInstruction:_simple_')
      elseif contains(19, feature.product) then
  			featurePortrayal:AddInstructions('PointInstruction:TRMLPG91')
  			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
        featurePortrayal:AddInstructions('LineInstruction:_simple_')
      else    -- catchall, should not execute unless condition in containing IF changes
  			featurePortrayal:AddInstructions('PointInstruction:TRMGEN91')
  			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
        featurePortrayal:AddInstructions('LineInstruction:_simple_')
      end
		elseif contains(1, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(3, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:TERMNL92')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		--[[
		elseif contains(4, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		]]--
		elseif contains(5, feature.categoryOfHarbourFacility) then
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
    elseif contains(7, feature.categoryOfHarbourFacility) then -- 7 tanker terminal
			featurePortrayal:AddInstructions('PointInstruction:TRMTNK91')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(8, feature.categoryOfHarbourFacility) then -- 8 passenger terminal
			featurePortrayal:AddInstructions('PointInstruction:TRMTNK91')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(10, feature.categoryOfHarbourFacility) then -- 10 container terminal
			featurePortrayal:AddInstructions('PointInstruction:TRMCON01')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(11, feature.categoryOfHarbourFacility) then -- 11 bulk terminal
			featurePortrayal:AddInstructions('PointInstruction:TERMNL94')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
      featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('PointInstruction:TRMGEN91')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
