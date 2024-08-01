
-- Referenced portrayal rules.
require 'DEPARE03'

-- Depth area main entry point.
function DepthArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 13030
		featurePortrayal:AddInstructions('ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRadar')
		
		if feature.depthRangeMinimumValue or feature.depthRangeMaximumValue then
			DEPARE03(feature, featurePortrayal, contextParameters, viewingGroup)
		else
			-- Incompletely surveyed area
			featurePortrayal:AddInstructions('ColorFill:NODTA')
			featurePortrayal:AddInstructions('AreaFillReference:PRTSUR01')
			featurePortrayal:SimpleLineStyle('solid',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
