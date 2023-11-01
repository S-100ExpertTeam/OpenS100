-- DepthNoBottomFound conditional symbology rules file.

-- UNOFFICIAL:  Rules extracted from S-52 rules for soundings with a qualityOfVerticalMeasurement of 5 - no bottom found at value shown.

-- Referenced CSPs.
require 'SNDFRM04'

-- Main entry point for CSP.
function DepthNoBottomFound(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.MultiPoint then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		local points = feature.MultiPoint.Points

		for i, point in ipairs(points) do
			local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, point, point.ScaledZ)

			featurePortrayal:AddInstructions('AugmentedPoint:GeographicCRS,' .. point.X .. ',' .. point.Y)

			for j, symbol in ipairs(symbols) do
				featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
			end
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:33010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		local point = feature.Point

		local symbols = SNDFRM04(feature, featurePortrayal, contextParameters, point, point.ScaledZ)

		featurePortrayal:AddInstructions('AugmentedPoint:GeographicCRS,' .. point.X .. ',' .. point.Y)

		for j, symbol in ipairs(symbols) do
			featurePortrayal:AddInstructions('PointInstruction:' .. symbol)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return 33010
end
