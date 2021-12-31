-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Sloping ground main entry point.
function SlopingGround(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 32010
		featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:HILTOP01')
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfSlope == 1 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 2 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 3 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 4 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 5 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 6 then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
		elseif feature.categoryOfSlope == 7 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR;NullInstruction')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfSlope == 1 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 2 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 3 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 4 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 5 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 6 then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfSlope == 7 and feature.radarConspicuous then
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHGRD')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32010
			featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR;NullInstruction')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
