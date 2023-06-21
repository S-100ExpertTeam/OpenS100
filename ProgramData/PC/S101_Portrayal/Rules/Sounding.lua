-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Referenced portrayal rules.
require 'SOUNDG03'

-- Sounding main entry point.
function Sounding(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.MultiPoint then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		SOUNDG03(feature, featurePortrayal, contextParameters)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
