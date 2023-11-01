-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Pipeline Submarine/On Land main entry point.
function PipelineSubmarineOnLand(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if contains(3, feature.product) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif contains(2, feature.categoryOfPipelinePipe) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif contains(3, feature.categoryOfPipelinePipe) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif contains(4, feature.categoryOfPipelinePipe) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		elseif contains(5, feature.categoryOfPipelinePipe) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		--
		-- issue #54, Bubble Curtain [Category of Pipe] (row 39 main)
		--
		elseif contains(7, feature.categoryOfPipelinePipe) then
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL06')
		-- end #54
		else
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:PIPSOL05')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
