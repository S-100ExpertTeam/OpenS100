-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2022/12/31)
-- For S-131 1.0.0

-- Referenced portrayal rules.
-- require 'DEPARE03'

-- Harbour Basin area main entry point.
function HarbourBasin(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		viewingGroup = 32410
    if contextParameters.RadarOverlay then
    	featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRADAR')
    else
    	featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRADAR')
    end
		featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:3;DisplayPlane:UnderRADAR')
    featurePortrayal:AddInstructions('ColorFill:DEPVS')
		featurePortrayal:AddInstructions('AreaFillReference:TRNBSN91')
		featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
