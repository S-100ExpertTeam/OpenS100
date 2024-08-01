require 'S101AttributeSupport'

-- Tunnel main entry point.
function Tunnel(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 32250
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:32250;DrawingPriority:12;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32250;DrawingPriority:12;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32250
		featurePortrayal:AddInstructions('ViewingGroup:32250;DrawingPriority:12;DisplayPlane:UnderRadar')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local featureName = GetFeatureName(feature, contextParameters)
	if featureName or HasHorizontalClearance(feature) then
		featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontColor:CHBLK')
		if featureName then
			featurePortrayal:AddTextInstruction(EncodeString(featureName), 26, 24, viewingGroup, 9)
			PortrayClearances(feature, featurePortrayal, contextParameters, viewingGroup, 0, -3.51)
		else
			PortrayClearances(feature, featurePortrayal, contextParameters, viewingGroup, 0, 0)
		end
	end

	return viewingGroup
end
