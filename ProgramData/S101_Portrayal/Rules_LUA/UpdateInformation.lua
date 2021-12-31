-- UpdateInformation conditional symbology rules file.

-- Main entry point for CSP.
function UpdateInformation(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRADAR')
		end

		featurePortrayal:AddInstructions('ViewingGroup:26200;DrawingPriority:4;PointInstruction:testPCB')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:AddInstructions('ViewingGroup:26200;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:26200;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
