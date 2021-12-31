-- TextPlacement portrayal rules file.

-- Main entry point for feature type.
function TextPlacement(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point then
		featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:7;DisplayPlane:OverRADAR')
		--featurePortrayal:AddInstructions('PointInstruction:POSGEN04')
		
		offsetValueX = 0.0
		if feature.textOffset then
			if feature.textOffset.valueOfX then
				offsetValueX = feature.textOffset.valueOfX:ToNumber()
			end
		end
		offsetValueY = 0.0
		if feature.textOffset then
			if feature.textOffset.valueOfY then
				offsetValueY = feature.textOffset.valueOfY:ToNumber()
			end
		end

		flipBearing = 0
		if feature.flipBearing then
			flipBearing = feature.flipBearing
		end

		if feature.text then
			featurePortrayal:AddInstructions('LocalOffset:' .. offsetValueX .. ',' .. offsetValueY .. ';TextAlignVertical:Center;TextAlignHorizontal:End;FontSize:10;FontSlant:'.. flipBearing ..';TextInstruction:' .. EncodeString(feature.text) .. ',26,8')
		end
	end
end