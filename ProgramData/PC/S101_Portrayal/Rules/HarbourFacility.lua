-- #155

-- Harbour facility main entry point.
function HarbourFacility(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if contains(1, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif contains(4, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif contains(5, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contains(1, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif contains(4, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif contains(5, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			viewingGroup = 32410
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:OverRadar')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(1, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif contains(4, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif contains(5, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(1, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif contains(4, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif contains(5, feature.categoryOfHarbourFacility) then
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			viewingGroup = 32410
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:12;DisplayPlane:UnderRadar')
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
