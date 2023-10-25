-- RestrictedAreaRegulatory main entry point.
-- PC #95
-- PC #105, PSWG #31

-- Referenced portrayal rules.
require 'RESARE04'

function RestrictedAreaRegulatory(feature, featurePortrayal, contextParameters)
	local viewingGroup = 26020

	if contains(14, feature.restriction) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53015,53015')
	elseif contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53016,53016')
	elseif not contains(14, feature.restriction) and not contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53014,53014')
	end

	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(27, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ESSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		elseif contains(28, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:PSSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			RESARE04(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(27, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ESSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		elseif contains(28, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:PSSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26020;DrawingPriority:15;DisplayPlane:UnderRADAR')
			RESARE04(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
