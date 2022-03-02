-- WindTurbine portrayal rules file.

-- Main entry point for feature type.
function WindTurbine(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		if feature.inTheWater then
			featurePortrayal:AddInstructions('ViewingGroup:12200')
		elseif feature.visuallyConspicuous == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:22220')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32220')
		end

		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DrawingPriority:18;DisplayPlane:OverRADAR;Hover:true')
		else
			featurePortrayal:AddInstructions('DrawingPriority:18;DisplayPlane:UnderRADAR;Hover:true')
		end

		featurePortrayal:AddInstructions('PointInstruction:WIMCON11')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
