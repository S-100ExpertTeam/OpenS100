-- TextPlacement portrayal rules file.

-- Main entry point for feature type.
function TextPlacement(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:7;DisplayPlane:OverRADAR')
		if feature.text then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignVertical:Center;TextAlignHorizontal:End;FontSize:10;FontSlant:'.. EncodeString(feature.flipBearing) ..';TextInstruction:' .. EncodeString(feature.text) .. ',26,8')
		end
	end
end
