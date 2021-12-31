-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Local magnetic anomaly main entry point.
function LocalMagneticAnomaly(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG01')
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG01')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG51')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:31080;DrawingPriority:4;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:LOCMAG51')
		featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
