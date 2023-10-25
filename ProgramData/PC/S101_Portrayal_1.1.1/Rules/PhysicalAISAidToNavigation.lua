-- PhysicalAISAidToNavigation portrayal rules file.

-- Issues [PSWG #70] #97

-- Main entry point for feature type.
function PhysicalAISAidToNavigation(feature, featurePortrayal, contextParameters)
	--error('Not Implemented: No symbology defined in S-52 for PhysicalAISAidToNavigation')

	local viewingGroup = 27240

	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:24;DisplayPlane:OverRADAR;Hover:true')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:24;DisplayPlane:UnderRADAR;Hover:true')
		end
		featurePortrayal:AddInstructions('PointInstruction:AISPHY05')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
