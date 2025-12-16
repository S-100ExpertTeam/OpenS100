-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- Modified for S-131 1.0.0 by RMM 20221231
-- S-131 1.0.0 does not have alerting or restriction or categoryOfAnchorage attributes

-- Anchorage area main entry point.
function AnchorageArea(feature, featurePortrayal, contextParameters)
	local viewingGroup
  local COA

--	featurePortrayal:AddInstructions('AlertReference:ProhAre,112,112')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 26220
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		-- there being no COA, the ACHARE02 symbol is always used
		featurePortrayal:AddInstructions('PointInstruction:ACHARE02')

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,-7.02;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 18)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 26220
		featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:ACHARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,-7.02;TextAlignHorizontal:End;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 9)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 26220
		featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:ACHARE51')
		featurePortrayal:AddInstructions('LineInstruction:ACHARE51')

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,7.02;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 26220, 9)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
