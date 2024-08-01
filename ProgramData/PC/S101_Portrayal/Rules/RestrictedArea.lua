
-- Referenced portrayal rules.
require 'RESARE04'

-- Restricted Area main entry point.
function RestrictedArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if contains(14, feature.restriction) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53015,53015')
	elseif contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53016,53016')
	elseif not contains(14, feature.restriction) and not contains(28, feature.categoryOfRestrictedArea) then
		featurePortrayal:AddInstructions('AlertReference:ProhAre,53014,53014')
	end

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries have same portrayal
		viewingGroup = 26010
		featurePortrayal:AddInstructions('ViewingGroup:26010;DrawingPriority:15;DisplayPlane:UnderRadar')
		if contains(27, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('PointInstruction:ESSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		elseif contains(28, feature.categoryOfRestrictedArea) then
			featurePortrayal:AddInstructions('PointInstruction:PSSARE01')
			featurePortrayal:AddInstructions('LineInstruction:ESSARE01')
		else
			RESARE04(feature, featurePortrayal, contextParameters, viewingGroup)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
