-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence

-- Issue #161, PSWG #108

-- Slope topline main entry point.
function SlopeTopline(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfSlope == 2 and feature.radarConspicuous then
			viewingGroup = 22210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 2 and feature.visualProminence == 1 then   -- #161
			viewingGroup = 22210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 6 and feature.radarConspicuous then
			viewingGroup = 22210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 6 and feature.visualProminence == 1 then
			viewingGroup = 22210
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22210;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 2 then
			viewingGroup = 32010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 6 then
			viewingGroup = 32010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32010
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
