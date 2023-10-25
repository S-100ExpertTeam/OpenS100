-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Submarine pipeline area main entry point.
function SubmarinePipelineArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 34030
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(2, feature.categoryOfPipelinePipe) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif contains(3, feature.categoryOfPipelinePipe) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif contains(3, feature.product) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(2, feature.categoryOfPipelinePipe) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif contains(3, feature.categoryOfPipelinePipe) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		elseif contains(3, feature.product) then
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			viewingGroup = 34030
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE51')
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
