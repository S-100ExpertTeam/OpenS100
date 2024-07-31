
-- Cable Submarine main entry point.
function CableSubmarine(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		-- issue #59 categoryOfCable 7 
		if feature.categoryOfCable == 6 or feature.categoryOfCable == 7 then
			viewingGroup = 24010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:24010;DrawingPriority:18;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:24010;DrawingPriority:18;DisplayPlane:UnderRadar')
			end
			if feature.categoryOfCable == 6 then
				-- mooring cable should generate alert since S-52 alerts on all MORFAC
				featurePortrayal:AddInstructions('AlertReference:NavHazard')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 34070
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:9;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34070;DrawingPriority:9;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('LineInstruction:CBLSUB06')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
