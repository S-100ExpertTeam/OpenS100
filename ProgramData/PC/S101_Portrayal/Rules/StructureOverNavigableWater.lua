require 'S101AttributeSupport'

function StructureOverNavigableWater(feature, featurePortrayal, contextParameters)
	local viewingGroup = 12210
	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Surface then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('ColorFill:CHGRF,0.5')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
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
