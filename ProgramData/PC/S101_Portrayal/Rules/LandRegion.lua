-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #194

-- Land region main entry point.
function LandRegion(feature, featurePortrayal, contextParameters)
	local viewingGroup = 21060

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:POSGEN04')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 12)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(2, feature.categoryOfLandRegion) then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		elseif contains(12, feature.categoryOfLandRegion) then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 9)
			else
				featurePortrayal:AddInstructions('NullInstruction')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(2, feature.categoryOfLandRegion) then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		elseif contains(12, feature.categoryOfLandRegion) then
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:MARSHES1')
		else
			featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 9)
			else
				featurePortrayal:AddInstructions('NullInstruction')
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		-- #235: implement LandRegion curve portrayal
		featurePortrayal:AddInstructions('ViewingGroup:21060;DrawingPriority:9;DisplayPlane:UnderRADAR')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21060, 9)
		else
			featurePortrayal:AddInstructions('NullInstruction')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
