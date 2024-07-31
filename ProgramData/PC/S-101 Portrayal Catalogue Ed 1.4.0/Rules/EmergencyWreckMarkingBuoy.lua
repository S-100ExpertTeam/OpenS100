-- BuoyEmergencyWreckMarking main entry point.
-- Feature Catalogue Version: 1.0.2 (2022/02/28)
-- #162
-- #276, [PSWG #117]

require 'TOPMAR02'

function EmergencyWreckMarkingBuoy(feature, featurePortrayal, contextParameters)
  local viewingGroup
  local symbol = 'BOYGEN03'

	featurePortrayal:AddInstructions('AlertReference:NavHazard;Hover:true')
  
  if feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 27010
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27010;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		
		if contextParameters.SimplifiedSymbols then
			symbol = 'BOYNDM02'
		else
			if feature.buoyShape == 3 then
				symbol = 'BOYSPH60'
			elseif feature.buoyShape == 4 then
				symbol = 'BOYPIL60'
			elseif feature.buoyShape == 5 then
				symbol = 'BOYSPR60'
			end
		end

		featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
		
		if not contextParameters.SimplifiedSymbols then
			TOPMAR02(feature, featurePortrayal, contextParameters, viewingGroup, true)		
		end

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:End;FontSize:10;FontColor:CHBLK')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), 'by %s'), 21, 24, 27010, 24)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end