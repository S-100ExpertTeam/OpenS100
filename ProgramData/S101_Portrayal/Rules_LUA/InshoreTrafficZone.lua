-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'RESTRN01'

-- Inshore traffic zone main entry point.
function InshoreTrafficZone(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:5;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:ITZARE51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'TRFCD')
		featurePortrayal:AddInstructions('LineInstruction:')
		RESTRN01(feature, featurePortrayal, contextParameters)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:5;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:ITZARE51')
		featurePortrayal:AddInstructions('LineInstruction:RESARE51')
		RESTRN01(feature, featurePortrayal, contextParameters)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
