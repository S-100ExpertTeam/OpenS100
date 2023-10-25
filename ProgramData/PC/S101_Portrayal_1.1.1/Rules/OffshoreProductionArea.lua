-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Issues PC #114, PSWG #63, #55, #67, #67, #54, extensively reworked from converter auto-generated

-- Referenced portrayal rules.
require 'RESTRN01'

-- Offshore production area main entry point.
function OffshoreProductionArea(feature, featurePortrayal, contextParameters)
	
	local viewingGroup = 26040
	local symbol = 'CTYARE51'	-- default or Seabed Material Extraction Area == 5

	local COPA = feature.categoryOfOffshoreProductionArea

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53018,53018')

	featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
	end

	-- set the symbol
	if feature.PrimitiveType == PrimitiveType.Surface then
		if contains (COPA, {2,3,6}) then		-- Wave, Current, or Solar Farm
			symbol = 'RENERG51'
		elseif COPA == 1 then					-- Wind Farm 
			symbol = 'WNDFRM52'
		elseif COPA == 4 then 
			symbol = 'TNKFRM'
		end

		featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
				
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
