-- WindTurbine portrayal rules file.
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence

-- Main entry point for feature type.
function WindTurbine(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		if feature.inTheWater then
			viewingGroup = 12200
			featurePortrayal:AddInstructions('ViewingGroup:12200')
			-- #365
			featurePortrayal:AddInstructions('AlertReference:NavHazard')
		elseif feature.visualProminence == 1 then
			viewingGroup = 22200
			featurePortrayal:AddInstructions('ViewingGroup:22220')
		else
			viewingGroup = 32200
			featurePortrayal:AddInstructions('ViewingGroup:32220')
		end

		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DrawingPriority:18;DisplayPlane:OverRadar;Hover:true')
		else
			featurePortrayal:AddInstructions('DrawingPriority:18;DisplayPlane:UnderRadar;Hover:true')
		end

		if feature.visualProminence == 1 then
			featurePortrayal:AddInstructions('PointInstruction:WIMCON11')
		else
			featurePortrayal:AddInstructions('PointInstruction:WIMCON01')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
