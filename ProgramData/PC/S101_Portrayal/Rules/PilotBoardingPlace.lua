-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- PSWG #106, PC #143 New Symbology for Pilot Boarding Place surface 

-- Pilot Boarding Place main entry point.
function PilotBoardingPlace(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 28010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:PILBOP02')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'Plt %s'), 21, 24, 28010, 18)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		viewingGroup = 28010
		featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:PILBARE51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'TRFCF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 28010
		featurePortrayal:AddInstructions('ViewingGroup:28010;DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:PILBARE51')
		featurePortrayal:AddInstructions('LineInstruction:CTYARE51')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
