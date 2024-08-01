
-- Referenced portrayal rules.
require 'QUAPOS01'

-- Land area main entry point.
function LandArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	-- DEBUG tessting #175
	--feature.featureName[1] = {nameUsage = nil, language = 'fre', name = 'FRE_LAN'}
	--feature.featureName[2] = {nameUsage = 1, language = 'eng', name = 'ENG_LAN'}
	--feature.featureName[3] = {nameUsage = nil, language = 'ger', name = 'GER_LAN'}

	featurePortrayal:AddInstructions('AlertReference:SafetyContour')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 12010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:LNDARE01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12010, 12)
		end
		QUAPOS01(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		QUAPOS01(feature, featurePortrayal, contextParameters, viewingGroup)
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,-3.51;TextAlignHorizontal:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12010, 24)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12010
		featurePortrayal:AddInstructions('ViewingGroup:12010;DrawingPriority:3;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('ColorFill:LANDA')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 12010, 3)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
