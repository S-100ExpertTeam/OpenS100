--
-- ISSUES: {PSWG #72, PC #121}, {PSWG #28, PC #124}
-- #169
-- #184
-- #206

-- Referenced portrayal rules.
require 'RESTRN01'

-- Anchorage area main entry point.
function AnchorageArea(feature, featurePortrayal, contextParameters)
	local viewingGroup
	
	local COA = feature.categoryOfAnchorage

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53023,53023')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26220
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:UnderRadar')
		end

		if contains (15, COA) then	-- 15 - Reported Anchorage PSWG #72
			featurePortrayal:AddInstructions('PointInstruction:ACHARE03')
		else
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		end

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,-7.02;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 18)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(15, COA) then -- #169 reported anchorage must have point geometry
			error('Surface is not a valid geometry for reported anchorages')
		else
			viewingGroup = 26220
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE51')
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')

			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,-7.02;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 9)
			end
			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(15, COA) then -- #169 reported anchorage must have point geometry
			error('Surface is not a valid geometry for reported anchorages')
		else
			viewingGroup = 26220
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE51')
			featurePortrayal:AddInstructions('LineInstruction:ACHARE51')

			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 9)
			end

			RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	return viewingGroup
end
