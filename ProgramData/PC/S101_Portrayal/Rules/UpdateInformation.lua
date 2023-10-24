-- UpdateInformation conditional symbology rules file.
-- #119
-- #192

-- Main entry point for CSP.
function UpdateInformation(feature, featurePortrayal, contextParameters)
	local viewingGroup = "updateReview"
	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRADAR')
		end

		featurePortrayal:AddInstructions('ViewingGroup:updateReview;DrawingPriority:12;PointInstruction:testPCB')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:AddInstructions('ViewingGroup:updateReview;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:updateReview;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	return viewingGroup
end
