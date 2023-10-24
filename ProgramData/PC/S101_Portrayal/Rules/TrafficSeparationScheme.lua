-- Main entry point for feature type.

-- Issue #146, PSWG #86
-- #61
-- #193

function TrafficSeparationScheme(feature, featurePortrayal, contextParameters)
	
	--featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') -- TODO: Find aggregates and emit instructions.

	local viewingGroup = 21060

	-- MS Debug: feature.featureName[1] = {displayName = true, language = 'eng', name = 'Traffic Separation Scheme'}
	
	 if feature.PrimitiveType == PrimitiveType.Surface then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:18;DisplayPlane:UnderRADAR')
		end

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters), '%s'), 21, 24, viewingGroup, 18)
		end
	elseif feature.PrimitiveType == PrimitiveType.None then
		featurePortrayal:AddInstructions('ViewingGroup:' .. viewingGroup .. ';DrawingPriority:15;DisplayPlane:UnderRADAR;NullInstruction') 
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
