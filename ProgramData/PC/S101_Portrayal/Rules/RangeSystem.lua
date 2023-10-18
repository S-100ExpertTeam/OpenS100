-- Main entry point for feature type.

-- Issue #147, PSWG #85
-- #61
-- #193

function RangeSystem(feature, featurePortrayal, contextParameters)
	
	--=featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') -- TODO: Find aggregates and emit instructions.

	local viewingGroup = 21060

	-- MS Debug: feature.featureName[1] = {displayName = true, language = 'eng', name = 'Range System'}

	 if feature.PrimitiveType == PrimitiveType.Surface or feature.PrimitiveType == PrimitiveType.Curve then

		-- Plain and symbolized boundaries use the same symbolization, text only
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup ..';DrawingPriority:15;DisplayPlane:UnderRADAR')

		if feature.featureName[1] and feature.featureName[1].name then
			if feature.PrimitiveType == PrimitiveType.Surface then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			elseif feature.PrimitiveType == PrimitiveType.Curve then
	 			featurePortrayal:AddInstructions('LocalOffset:3.51,0;FontSize:10')
			end 
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), '%s'), 21, 24, viewingGroup, 15)
		end
	elseif feature.PrimitiveType == PrimitiveType.None then
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') 
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
