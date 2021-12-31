-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Administration Area main entry point.
function AdministrationArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:2;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:36050;DrawingPriority:2;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:ADMARE01')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
