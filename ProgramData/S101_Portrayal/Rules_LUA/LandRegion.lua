-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Land region main entry point.
function LandRegion(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:POSGEN04')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfLandRegion[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		elseif feature.categoryOfLandRegion[1] == 12 then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfLandRegion[1] == 2 then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		elseif feature.categoryOfLandRegion[1] == 12 then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:3;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
			end
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
