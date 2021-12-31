-- Default portrayal rules file.  Called when rule file cannot be found.

-- Main entry point for feature type.
function Default(feature, featurePortrayal, contextParameters)
	if (feature.PrimitiveType == PrimitiveType.Point or feature.PrimitiveType == PrimitiveType.MultiPoint) then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5')
		featurePortrayal:AddInstructions('PointInstruction:QUESMRK1')
	elseif (feature.PrimitiveType == PrimitiveType.Curve) then
		if contextParameters.RADAR_OVERLAY then
			featurePortrayal:AddInstructions('DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5')
		featurePortrayal:AddInstructions('LineInstruction:QUESMRK1')
	elseif (feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES) then
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:QUESMRK1')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
	elseif (feature.PrimitiveType == PrimitiveType.Surface) then
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:5;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:QUESMRK1')
		featurePortrayal:AddInstructions('LineInstruction:QUESMRK1')
	end
end
