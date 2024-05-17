-- Default portrayal rules file.  Called when rule file cannot be found.
-- #119

-- Main entry point for feature type.
function Default(feature, featurePortrayal, contextParameters)

	if (feature.PrimitiveType == PrimitiveType.Point or feature.PrimitiveType == PrimitiveType.MultiPoint) then
		-- Simplified and paper chart points use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:21010,unknown;DrawingPriority:15;DisplayPlane:OverRADAR')
		featurePortrayal:AddInstructions('PointInstruction:USRPNT01')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21010, 3)
		end
	elseif (feature.PrimitiveType == PrimitiveType.Curve) then
		featurePortrayal:AddInstructions('ViewingGroup:21010,unknown;DrawingPriority:15;DisplayPlane:OverRADAR')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21010, 3)
		end
	elseif (feature.PrimitiveType == PrimitiveType.Surface) then
		featurePortrayal:AddInstructions('ViewingGroup:21010,unknown;DrawingPriority:15;DisplayPlane:OverRADAR')
		featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('ColorFill:DNGHL')
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:-3.51,3.51;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
			featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 21010, 3)
		end
	end
	return "21010,unknown"
end
