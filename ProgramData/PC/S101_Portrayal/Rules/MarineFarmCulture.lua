-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Marine farm/culture main entry point.
function MarineFarmCulture(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53024,53024')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:MARCUL02')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 26210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26210
		featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('AreaFillReference:MARCUL02')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26210
		featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('AreaFillReference:MARCUL02')
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
