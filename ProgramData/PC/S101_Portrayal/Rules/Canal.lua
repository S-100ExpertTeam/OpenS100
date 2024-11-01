require 'S101AttributeSupport'

-- Canal main entry point.
function Canal(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12420
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.condition then
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.condition then
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 12420
			featurePortrayal:AddInstructions('ViewingGroup:12420;DrawingPriority:6;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('ColorFill:DEPVS')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
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
