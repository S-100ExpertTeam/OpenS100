-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Pipeline Submarine/On Land main entry point.
function PipelineSubmarineOnLand(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('NullInstruction')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.product[1] == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif feature.categoryOfPipelinePipe[1] == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif feature.categoryOfPipelinePipe[1] == 3 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif feature.categoryOfPipelinePipe[1] == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif feature.categoryOfPipelinePipe[1] == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:6;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL05')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
