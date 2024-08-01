-- #155

-- Light vessel main entry point.
function LightVessel(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		viewingGroup = 27011
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:LITVES02')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'LtV %s'), 21, 24, 27011, 24)
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 27011
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:LITVES01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'LtV %s'), 21, 24, 27011, 24)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
