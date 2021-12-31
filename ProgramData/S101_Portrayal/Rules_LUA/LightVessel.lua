-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Light vessel main entry point.
function LightVessel(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:LITVES02')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'LtV %s') .. ',21,8')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27011;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:LITVES01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters), 'LtV %s') .. ',21,8')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
