-- Chart1Feature main entry point.
function Chart1Feature(feature, featurePortrayal, contextParameters)

	if feature.PrimitiveType ~= PrimitiveType.Point and feature.PrimitiveType ~= PrimitiveType.Curve and feature.PrimitiveType ~= PrimitiveType.Surface then
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local viewingGroup = 21020
	
	featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')

	local featureName = EncodeString(GetFeatureName(feature, contextParameters))
	if featureName then
		-- Add default text parameters
		featurePortrayal:AddInstructions('TextAlignHorizontal:Center;TextAlignVertical:Center;FontColor:CHBLK')
	end	

	if feature.drawingInstruction then
		for _,instructions in ipairs(feature.drawingInstruction) do
			-- may modify the default text parameters set above
			featurePortrayal:AddInstructions(instructions)
		end
	end

	if featureName then
		-- args: text, textViewingGroup, textPriority, featureViewingGroup
        featurePortrayal:AddTextInstruction(featureName, 21, 24, viewingGroup)
    end

	return viewingGroup
end