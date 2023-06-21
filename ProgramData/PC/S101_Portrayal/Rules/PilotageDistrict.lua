-- PilotageDistrict portrayal rules file.

-- Main entry point for feature type.
function PilotageDistrict(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:12;DisplayPlane:UnderRADAR')

		featurePortrayal:AddInstructions('PointInstruction:testPCB')
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
