-- BuoyEmergencyWreckMarking main entry point.
-- Feature Catalogue Version: 1.0.2 (2022/02/28)
-- #162

function BuoyEmergencyWreckMarking(feature, featurePortrayal, contextParameters)
  local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard;Hover:true')
  
  if feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 27010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:BOYNDM01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), 21, 24, 27010, 24)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end