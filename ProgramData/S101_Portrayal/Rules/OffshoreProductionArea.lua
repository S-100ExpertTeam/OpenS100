-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Offshore production area main entry point.
function OffshoreProductionArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:ProhAre,106,106')

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26040
		featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
		--featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
		featurePortrayal:AddInstructions('PointInstruction:2021_WaveFarm')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfOffshoreProductionArea == 2 then
			viewingGroup = 26040
			featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
			featurePortrayal:AddInstructions('PointInstruction:2021_WaveFarm')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51_CHGRD')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif feature.categoryOfOffshoreProductionArea == 4 then
			viewingGroup = 26040
			featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
			--featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
			featurePortrayal:AddInstructions('PointInstruction:2021_TankFarm')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 26040
			featurePortrayal:AddInstructions('ViewingGroup:26040;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CTYARE51')
			featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
