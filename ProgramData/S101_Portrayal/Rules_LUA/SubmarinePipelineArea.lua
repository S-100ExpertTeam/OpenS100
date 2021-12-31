-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Submarine pipeline area main entry point.
function SubmarinePipelineArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfPipelinePipe[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			RESTRN01(feature, featurePortrayal, contextParameters)
		elseif feature.categoryOfPipelinePipe[1] == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			RESTRN01(feature, featurePortrayal, contextParameters)
		elseif feature.product[1] == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
			RESTRN01(feature, featurePortrayal, contextParameters)
		else
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:')
			RESTRN01(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfPipelinePipe[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters)
		elseif feature.categoryOfPipelinePipe[1] == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters)
		elseif feature.product[1] == 3 then
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE61')
			RESTRN01(feature, featurePortrayal, contextParameters)
		else
			featurePortrayal:AddInstructions('ViewingGroup:34030;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:INFARE51')
			featurePortrayal:AddInstructions('LineInstruction:PIPARE51')
			RESTRN01(feature, featurePortrayal, contextParameters)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
