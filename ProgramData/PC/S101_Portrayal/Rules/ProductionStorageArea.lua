-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
-- #155

-- Production/storage area main entry point.
function ProductionStorageArea(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if feature.categoryOfProductionArea == 5 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK11')
		elseif feature.categoryOfProductionArea == 8 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif feature.categoryOfProductionArea == 9 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WIMCON11')
		elseif feature.categoryOfProductionArea == 5 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK01')
		elseif feature.categoryOfProductionArea == 6 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TMBYRD01')
		elseif feature.categoryOfProductionArea == 8 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		elseif feature.categoryOfProductionArea == 9 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WIMCON01')
		else
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:PRDINS02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfProductionArea == 5 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK11')
		elseif feature.categoryOfProductionArea == 8 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TNKCON12')
		elseif feature.categoryOfProductionArea == 9 and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WIMCON11')
		elseif feature.categoryOfProductionArea == 5 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK01')
		elseif feature.categoryOfProductionArea == 6 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TMBYRD01')
		elseif feature.categoryOfProductionArea == 8 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TNKCON02')
		elseif feature.categoryOfProductionArea == 9 then
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WIMCON01')
		else
			viewingGroup = 32270
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:PRDINS02')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.categoryOfProductionArea == 5 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RFNERY11')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 8 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TNKFRM11')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 9 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:WNDFRM61')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 1 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:QUARRY01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 5 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RFNERY01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 6 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TMBYRD01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 8 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TNKFRM01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 9 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:WNDFRM51')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfProductionArea == 5 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RFNERY11')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 8 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TNKFRM11')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 9 and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:WNDFRM61')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 1 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:QUARRY01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 5 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:RFNERY01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 6 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TMBYRD01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 8 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:TNKFRM01')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.categoryOfProductionArea == 9 then
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:WNDFRM51')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32270
			featurePortrayal:AddInstructions('ViewingGroup:32270;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:SimpleLineStyle('dash',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
