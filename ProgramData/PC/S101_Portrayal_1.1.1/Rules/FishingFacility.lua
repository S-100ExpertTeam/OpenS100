-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #155

-- Fishing facility main entry point.
function FishingFacility(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('AlertReference:NavHazard')

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfFishingFacility == 1 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC03')
		elseif feature.categoryOfFishingFacility == 2 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		elseif feature.categoryOfFishingFacility == 3 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		elseif feature.categoryOfFishingFacility == 4 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		else
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHHAV01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfFishingFacility == 1 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC03')
		elseif feature.categoryOfFishingFacility == 2 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		elseif feature.categoryOfFishingFacility == 3 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		elseif feature.categoryOfFishingFacility == 4 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
		else
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FSHHAV01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfFishingFacility == 1 then
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('LineInstruction:FSHFAC02')
		else
			viewingGroup = 26210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfFishingFacility == 1 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:FSHFAC03')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfFishingFacility == 2 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:FSHFAC04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfFishingFacility == 3 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:FSHFAC04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfFishingFacility == 4 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:FSHFAC04')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:FSHHAV02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfFishingFacility == 1 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC03')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.categoryOfFishingFacility == 2 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.categoryOfFishingFacility == 3 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.categoryOfFishingFacility == 4 then
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		else
			viewingGroup = 26210
			featurePortrayal:AddInstructions('ViewingGroup:26210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FSHFAC02')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
