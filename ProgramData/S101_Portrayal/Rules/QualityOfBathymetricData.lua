-- QualityOfBathymetricData portrayal rules file.

-- Main entry point for feature type.
function QualityOfBathymetricData(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType ~= PrimitiveType.Surface then
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local function DepthAccuracy(a, b)
		local d = feature.depthRangeMinimumValue

		if d then
			local accuracy = a + (b * d) / SD(100)

			return feature.verticalUncertainty.uncertaintyFixed <= accuracy
		end
	end

	-- Determine CATZOC equivalence.  See S-57 Appendix A Chapter 2.

	local scaleFactor = 0.311 -- Scale 16.04mm symbols to 5mm.

	local catzoc = 'U01'

	if (feature.dataAssessment == 1 or feature.dataAssessment == 2) and feature.horizontalPositionUncertainty.uncertaintyFixed and feature.verticalUncertainty.uncertaintyFixed then
		catzoc = 'D01'

		local hpu = feature.horizontalPositionUncertainty.uncertaintyFixed

		if hpu <= SD(500) and DepthAccuracy(SD(2), SD(5)) then
			catzoc = 'C01'
		end

		if hpu <= SD(50) and DepthAccuracy(SD(1), SD(2)) then
			scaleFactor = 0.294 -- Scale 16.97mm symbols to 5mm.
			catzoc = 'B01'
		end

		if feature.fullSeafloorCoverageAchieved and hpu <= SD(20) and DepthAccuracy(SD(1), SD(2)) then
			scaleFactor = 0.294 -- Scale 16.97mm symbols to 5mm.
			catzoc = 'A21'
		end

		if feature.fullSeafloorCoverageAchieved and hpu <= SD(5) and DepthAccuracy(SD(5, 1), SD(1)) then
			scaleFactor = 0.294 -- Scale 16.97mm symbols to 5mm.
			catzoc = 'A11'
		end
	end

	featurePortrayal:AddInstructions('ViewingGroup:31010;DrawingPriority:12;DisplayPlane:UnderRADAR')
	featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
	featurePortrayal:AddInstructions('LineInstruction:_simple_;AreaFillReference:testPCB')
	--featurePortrayal:AddInstructions('LineInstruction:_simple_;ScaleFactor:' .. scaleFactor .. ';PointInstruction:DQUAL' .. catzoc .. 'P')

	return 31010
end
