-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- FC 1.0.1: manually changed visuallyConspicuous to visualProminence
--
-- Issues: PSWG #60, PC #104 Torii
-- Issues: PSWG #62, PC #107 Observation wheel
-- Issues: PSWG #66, PC #108 Triangulation Mark
-- #155
-- #24: use FLGSTF02 for conspicuous flagstaff
-- #191

-- Landmark main entry point.
function Landmark(feature, featurePortrayal, contextParameters)
	local viewingGroup

	featurePortrayal:AddInstructions('Hover:true')
		
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SimplifiedSymbols then
		if contains(15, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(15, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(31, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS15')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 18)
			end
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 18)
			end
		elseif contains(20, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(20, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(20, feature.categoryOfLandmark) and contains(26, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(20, feature.categoryOfLandmark) and contains(27, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(1, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
		elseif contains(3, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHIMNY11')
		elseif contains(4, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DSHAER11')
		elseif contains(5, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLGSTF02')
		elseif contains(6, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK11')
		elseif contains(7, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MSTCON14')
		elseif contains(8, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(9, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(10, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(12, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(13, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(15, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DOMES011')
		elseif contains(16, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RASCAN11')
		elseif contains(17, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS03')
		elseif contains(18, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WNDMIL12')
		elseif contains(20, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(24, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			-- Issues: PSWG #62, PC #107 (NOT AUTO GENERATED), Observation wheel
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FERWHL03')
		elseif contains(25, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			-- Issues: PSWG #60, PC #104 (NOT AUTO GENERATED)
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TORII11')
		elseif contains(15, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(17, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(17, feature.categoryOfLandmark) and contains(31, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS05')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
		elseif contains(20, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(1, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
		elseif contains(3, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHIMNY01')
		elseif contains(4, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DSHAER01')
		elseif contains(5, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLGSTF01')
		elseif contains(6, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK01')
		elseif contains(7, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MSTCON04')
		elseif contains(8, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(9, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(10, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(12, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(13, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(15, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DOMES001')
		elseif contains(16, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RASCAN01')
		elseif contains(17, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS01')
		elseif contains(18, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WNDMIL02')
		elseif contains(20, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		elseif contains(22, feature.categoryOfLandmark) then
			-- Issues: PSWG #66, PC #108 (NOT AUTO GENERATED), Triangulation Mark
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN05')
		elseif contains(24, feature.categoryOfLandmark) then
			-- Issues: PSWG #62, PC #107 (NOT AUTO GENERATED), Observation wheel
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FERWHL04')
		elseif contains(25, feature.categoryOfLandmark) then
			-- Issues: PSWG #60, PC #104 (NOT AUTO GENERATED)
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TORII01')
		else
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if contains(15, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(15, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(17, feature.categoryOfLandmark) and contains(31, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS15')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 18)
			end
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS03')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 18)
			end
		elseif contains(20, feature.categoryOfLandmark) and contains(20, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(20, feature.categoryOfLandmark) and contains(21, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL13')
		elseif contains(20, feature.categoryOfLandmark) and contains(26, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(20, feature.categoryOfLandmark) and contains(27, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL15')
		elseif contains(1, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS11')
		elseif contains(3, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHIMNY11')
		elseif contains(4, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DSHAER11')
		elseif contains(5, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLGSTF02')
		elseif contains(6, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK11')
		elseif contains(7, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MSTCON14')
		elseif contains(8, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(9, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(10, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(12, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(13, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT12')
		elseif contains(15, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DOMES011')
		elseif contains(16, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RASCAN11')
		elseif contains(17, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS03')
		elseif contains(18, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WNDMIL12')
		elseif contains(20, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(24, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			-- Issues: PSWG #62, PC #107 (NOT AUTO GENERATED), Observation wheel
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FERWHL03')
		elseif contains(25, feature.categoryOfLandmark) and feature.visualProminence == 1 then
			-- Issues: PSWG #60, PC #104 (NOT AUTO GENERATED)
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TORII11')
		elseif contains(15, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(17, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif contains(17, feature.categoryOfLandmark) and contains(31, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS05')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,-3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS01')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:3.51,3.51;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
		elseif contains(20, feature.categoryOfLandmark) and contains(20, feature['function']) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:BUIREL01')
		elseif feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:18;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(1, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CAIRNS01')
		elseif contains(3, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:CHIMNY01')
		elseif contains(4, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DSHAER01')
		elseif contains(5, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLGSTF01')
		elseif contains(6, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLASTK01')
		elseif contains(7, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MSTCON04')
		elseif contains(8, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN03')
		elseif contains(9, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(10, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(12, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(13, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:MONUMT02')
		elseif contains(15, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:DOMES001')
		elseif contains(16, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:RASCAN01')
		elseif contains(17, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TOWERS01')
		elseif contains(18, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:WNDMIL02')
		elseif contains(20, feature.categoryOfLandmark) then
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		elseif contains(22, feature.categoryOfLandmark) then
			-- Issues: PSWG #66, PC #108 (NOT AUTO GENERATED), Triangulation Mark
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN05')
		elseif contains(24, feature.categoryOfLandmark) then
			-- Issues: PSWG #62, PC #107 (NOT AUTO GENERATED), Observation wheel
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FERWHL04')
		elseif contains(25, feature.categoryOfLandmark) then
			-- Issues: PSWG #60, PC #104 (NOT AUTO GENERATED)
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:TORII01')
		else
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.visualProminence == 1 then
			viewingGroup = 22220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32220
			if contextParameters.RadarOverlay then
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 12)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) then
			viewingGroup = 32220
			featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32220
			featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) and feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 22220, 12)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif contains(17, feature.categoryOfLandmark) and contains(33, feature['function']) then
			viewingGroup = 32220
			featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			if feature.featureName[1] and feature.featureName[1].name then
				featurePortrayal:AddInstructions('LocalOffset:0,0;TextAlignHorizontal:Center;TextAlignVertical:Center;FontSize:10')
				featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, 32220, 12)
			end
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		elseif feature.visualProminence == 1 then
			viewingGroup = 22220
			featurePortrayal:AddInstructions('ViewingGroup:22220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'CHBLK')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			viewingGroup = 32220
			featurePortrayal:AddInstructions('ViewingGroup:32220;DrawingPriority:12;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('ColorFill:CHBRN')
			featurePortrayal:SimpleLineStyle('solid',0.32,'LANDF')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	return viewingGroup 
end
