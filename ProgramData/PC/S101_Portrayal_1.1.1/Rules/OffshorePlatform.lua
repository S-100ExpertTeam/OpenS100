-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Offshore platform main entry point.
function OffshorePlatform(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 12210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:OFSPLF01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Prod %s'), 21, 24, 12210, 15)
		end
		-- #63 Flare Stack (row 52 main)
		-- Debug.Break()
		-- debug feature.flareStack = 1

		if feature.flareStack then
			--featurePortrayal:AddInstructions('LocalOffset:3.51,-1.25;FontSize:10')
			featurePortrayal:AddInstructions('LocalOffset:3.51,0;TextAlignHorizontal:Start;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString('Fla','%s'), 21, 24, 12210, 15)
		end
		-- end #63
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12210
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',1.28,'CSTLN')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Prod %s'), 21, 24, 12210, 15)
		end
		-- #63 Flare Stack (row 52 main)
		-- debug feature.flareStack = 1

		if feature.flareStack then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString('Fla','%s'), 21, 24, 12210, 15)
		end
		-- end #63

	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
