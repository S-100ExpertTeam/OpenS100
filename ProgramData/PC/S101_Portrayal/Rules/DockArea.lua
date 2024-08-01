require 'S101AttributeSupport'

-- Dock area main entry point.
function DockArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		viewingGroup = 22010
		featurePortrayal:AddInstructions('ViewingGroup:22010;DrawingPriority:9;DisplayPlane:UnderRadar;ColorFill:DEPVS')
		-- #269: Dock area outline has incorrect priority and viewing group
		if feature.condition then
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
		else
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
		end
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
