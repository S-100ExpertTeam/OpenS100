-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Slope topline main entry point.
function SlopeTopline(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.visuallyConspicuous == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HILTOP11')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HILTOP01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.visuallyConspicuous == 1 then
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HILTOP11')
		else
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:3;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:HILTOP01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfSlope == 2 and feature.radarConspicuous then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfSlope == 6 and feature.radarConspicuous then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfSlope == 6 and feature.visuallyConspicuous == 1 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfSlope == 2 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfSlope == 6 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
