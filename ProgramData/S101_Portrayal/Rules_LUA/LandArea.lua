-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'QUAPOS01'

-- Land area main entry point.
function LandArea(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:4;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:4;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:LNDARE01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
		end
		QUAPOS01(feature, featurePortrayal, contextParameters)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:8;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:8;DisplayPlane:UnderRADAR')
		end
		QUAPOS01(feature, featurePortrayal, contextParameters)
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:1;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:LANDA')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;TextInstruction:' .. EncodeString(GetFeatureName(feature, contextParameters)) .. ',26,8')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
