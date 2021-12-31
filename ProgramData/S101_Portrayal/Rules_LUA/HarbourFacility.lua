-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Harbour facility main entry point.
function HarbourFacility(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.categoryOfHarbourFacility[1] == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif feature.categoryOfHarbourFacility[1] == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif feature.categoryOfHarbourFacility[1] == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfHarbourFacility[1] == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif feature.categoryOfHarbourFacility[1] == 4 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif feature.categoryOfHarbourFacility[1] == 5 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfHarbourFacility[1] == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif feature.categoryOfHarbourFacility[1] == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif feature.categoryOfHarbourFacility[1] == 5 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfHarbourFacility[1] == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ROLROL01')
		elseif feature.categoryOfHarbourFacility[1] == 4 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HRBFAC09')
		elseif feature.categoryOfHarbourFacility[1] == 5 then
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:SMCFAC02')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CHINFO07')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
