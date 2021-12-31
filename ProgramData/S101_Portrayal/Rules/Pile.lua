-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Pile main entry point.
function Pile(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard,115,115')

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 12410
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:15;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:15;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:PILPNT02')		
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		--TODO: Determine S-101 portrayal for Pile curve feature.
		--viewingGroup = 21010
		--featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:15;PointInstruction:testPCB')
		--Debug.Trace('Warning: S-52 does not define portrayal for Pile curve features.')
		viewingGroup = 21010
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:PILE_2021')	
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		--TODO: Determine S-101 portrayal for Pile surface feature.
		viewingGroup = 21010
		featurePortrayal:AddInstructions('ViewingGroup:21010;DrawingPriority:18;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('LineInstruction:PILE_2021')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
