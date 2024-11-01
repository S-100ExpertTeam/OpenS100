
-- Cable Submarine main entry point.
function CableSubmarine(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then

		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		end

		if feature.categoryOfCable == 9 then
			-- issue #444 junction cable symbolized incorrectly
			viewingGroup = 14020
			featurePortrayal:AddInstructions('AlertReference:NavHazard;ViewingGroup:14010;DrawingPriority:18')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')			
		elseif feature.categoryOfCable == 6 or feature.categoryOfCable == 7 then
			-- issue #59 categoryOfCable 7
			viewingGroup = 24010
			featurePortrayal:AddInstructions('ViewingGroup:24010;DrawingPriority:18')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 34070
			featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:9')
			featurePortrayal:AddInstructions('LineInstruction:CBLSUB06')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
