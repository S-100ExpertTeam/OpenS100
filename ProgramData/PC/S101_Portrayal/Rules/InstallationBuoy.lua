-- #155
-- #238 [PSWG #117], no changes needed here


-- Buoy Installation main entry point.
function InstallationBuoy(feature, featurePortrayal, contextParameters)
	local viewingGroup = 27010

	featurePortrayal:AddInstructions('AlertReference:NavHazard;Hover:true')

	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		if contextParameters.SimplifiedSymbols then
			featurePortrayal:AddInstructions('PointInstruction:BOYMOR11')
		else
			featurePortrayal:AddInstructions('PointInstruction:BOYINB01')
		end
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), 21, 24, viewingGroup, 24)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
