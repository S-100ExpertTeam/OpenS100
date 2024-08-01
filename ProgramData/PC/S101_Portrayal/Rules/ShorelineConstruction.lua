-- Referenced portrayal rules.
require 'SLCONS04'
require 'S101AttributeSupport'

-- Shoreline construction main entry point.
function ShorelineConstruction(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:SafetyContour')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 12410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:MORFAC03')
		SLCONS04(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		viewingGroup = 12410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:21;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:21;DisplayPlane:UnderRadar')
		end
		SLCONS04(feature, featurePortrayal, contextParameters, viewingGroup)
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 12410
		featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:21;DisplayPlane:UnderRadar')
		SLCONS04(feature, featurePortrayal, contextParameters, viewingGroup)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local featureName = GetFeatureName(feature, contextParameters)
	if featureName or HasHorizontalClearance(feature) then
		-- Center annotationas in or on the feature
		local yOffset = 0
		if feature.PrimitiveType == PrimitiveType.Point then
			-- Center annotations underneath the symbol
			featurePortrayal:AddInstructions('TextAlignVertical:Center')
			yOffset = -3.51
		else
			featurePortrayal:AddInstructions('TextAlignVertical:Center')
		end
		featurePortrayal:AddInstructions('LocalOffset:0,' .. yOffset .. ';TextAlignHorizontal:Center;FontColor:CHBLK')
		if featureName then
			featurePortrayal:AddTextInstruction(EncodeString(featureName), 26, 24, viewingGroup, 9)
			yOffset = yOffset - 3.51
		end
		PortrayClearances(feature, featurePortrayal, contextParameters, viewingGroup, 0, yOffset)
	end

	return viewingGroup
end
